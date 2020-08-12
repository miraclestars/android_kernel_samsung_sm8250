// SPDX-License-Identifier: GPL-2.0
/*
 * property.c - Unified device property interface.
 *
 * Copyright (C) 2014, Intel Corporation
 * Authors: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 *          Mika Westerberg <mika.westerberg@linux.intel.com>
 */

#include <linux/acpi.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_graph.h>
#include <linux/of_irq.h>
#include <linux/property.h>
#include <linux/etherdevice.h>
#include <linux/phy.h>

struct property_set {
	struct device *dev;
	struct fwnode_handle fwnode;
	const struct property_entry *properties;
};

static const struct fwnode_operations pset_fwnode_ops;

static inline bool is_pset_node(const struct fwnode_handle *fwnode)
{
	return !IS_ERR_OR_NULL(fwnode) && fwnode->ops == &pset_fwnode_ops;
}

#define to_pset_node(__fwnode)						\
	({								\
		typeof(__fwnode) __to_pset_node_fwnode = __fwnode;	\
									\
		is_pset_node(__to_pset_node_fwnode) ?			\
			container_of(__to_pset_node_fwnode,		\
				     struct property_set, fwnode) :	\
			NULL;						\
	})

static const struct property_entry *
pset_prop_get(const struct property_set *pset, const char *name)
{
	const struct property_entry *prop;

	if (!pset || !pset->properties)
		return NULL;

	for (prop = pset->properties; prop->name; prop++)
		if (!strcmp(name, prop->name))
			return prop;

	return NULL;
}

static const void *property_get_pointer(const struct property_entry *prop)
{
	switch (prop->type) {
	case DEV_PROP_U8:
		if (prop->is_array)
			return prop->pointer.u8_data;
		return &prop->value.u8_data;
	case DEV_PROP_U16:
		if (prop->is_array)
			return prop->pointer.u16_data;
		return &prop->value.u16_data;
	case DEV_PROP_U32:
		if (prop->is_array)
			return prop->pointer.u32_data;
		return &prop->value.u32_data;
	case DEV_PROP_U64:
		if (prop->is_array)
			return prop->pointer.u64_data;
		return &prop->value.u64_data;
	case DEV_PROP_STRING:
		if (prop->is_array)
			return prop->pointer.str;
		return &prop->value.str;
	default:
		return NULL;
	}
}

static void property_set_pointer(struct property_entry *prop, const void *pointer)
{
	switch (prop->type) {
	case DEV_PROP_U8:
		if (prop->is_array)
			prop->pointer.u8_data = pointer;
		else
			prop->value.u8_data = *((u8 *)pointer);
		break;
	case DEV_PROP_U16:
		if (prop->is_array)
			prop->pointer.u16_data = pointer;
		else
			prop->value.u16_data = *((u16 *)pointer);
		break;
	case DEV_PROP_U32:
		if (prop->is_array)
			prop->pointer.u32_data = pointer;
		else
			prop->value.u32_data = *((u32 *)pointer);
		break;
	case DEV_PROP_U64:
		if (prop->is_array)
			prop->pointer.u64_data = pointer;
		else
			prop->value.u64_data = *((u64 *)pointer);
		break;
	case DEV_PROP_STRING:
		if (prop->is_array)
			prop->pointer.str = pointer;
		else
			prop->value.str = pointer;
		break;
	default:
		break;
	}
}

static const void *pset_prop_find(const struct property_set *pset,
				  const char *propname, size_t length)
{
	const struct property_entry *prop;
	const void *pointer;

	prop = pset_prop_get(pset, propname);
	if (!prop)
		return ERR_PTR(-EINVAL);
	pointer = property_get_pointer(prop);
	if (!pointer)
		return ERR_PTR(-ENODATA);
	if (length > prop->length)
		return ERR_PTR(-EOVERFLOW);
	return pointer;
}

static int pset_prop_read_u8_array(const struct property_set *pset,
				   const char *propname,
				   u8 *values, size_t nval)
{
	const void *pointer;
	size_t length = nval * sizeof(*values);

	pointer = pset_prop_find(pset, propname, length);
	if (IS_ERR(pointer))
		return PTR_ERR(pointer);

	memcpy(values, pointer, length);
	return 0;
}

static int pset_prop_read_u16_array(const struct property_set *pset,
				    const char *propname,
				    u16 *values, size_t nval)
{
	const void *pointer;
	size_t length = nval * sizeof(*values);

	pointer = pset_prop_find(pset, propname, length);
	if (IS_ERR(pointer))
		return PTR_ERR(pointer);

	memcpy(values, pointer, length);
	return 0;
}

static int pset_prop_read_u32_array(const struct property_set *pset,
				    const char *propname,
				    u32 *values, size_t nval)
{
	const void *pointer;
	size_t length = nval * sizeof(*values);

	pointer = pset_prop_find(pset, propname, length);
	if (IS_ERR(pointer))
		return PTR_ERR(pointer);

	memcpy(values, pointer, length);
	return 0;
}

static int pset_prop_read_u64_array(const struct property_set *pset,
				    const char *propname,
				    u64 *values, size_t nval)
{
	const void *pointer;
	size_t length = nval * sizeof(*values);

	pointer = pset_prop_find(pset, propname, length);
	if (IS_ERR(pointer))
		return PTR_ERR(pointer);

	memcpy(values, pointer, length);
	return 0;
}

static int pset_prop_count_elems_of_size(const struct property_set *pset,
					 const char *propname, size_t length)
{
	const struct property_entry *prop;

	prop = pset_prop_get(pset, propname);
	if (!prop)
		return -EINVAL;

	return prop->length / length;
}

static int pset_prop_read_string_array(const struct property_set *pset,
				       const char *propname,
				       const char **strings, size_t nval)
{
	const struct property_entry *prop;
	const void *pointer;
	size_t array_len, length;

	/* Find out the array length. */
	prop = pset_prop_get(pset, propname);
	if (!prop)
		return -EINVAL;

	if (!prop->is_array)
		/* The array length for a non-array string property is 1. */
		array_len = 1;
	else
		/* Find the length of an array. */
		array_len = pset_prop_count_elems_of_size(pset, propname,
							  sizeof(const char *));

	/* Return how many there are if strings is NULL. */
	if (!strings)
		return array_len;

	array_len = min(nval, array_len);
	length = array_len * sizeof(*strings);

	pointer = pset_prop_find(pset, propname, length);
	if (IS_ERR(pointer))
		return PTR_ERR(pointer);

	memcpy(strings, pointer, length);

	return array_len;
}

struct fwnode_handle *dev_fwnode(struct device *dev)
{
	return IS_ENABLED(CONFIG_OF) && dev->of_node ?
		&dev->of_node->fwnode : dev->fwnode;
}
EXPORT_SYMBOL_GPL(dev_fwnode);

static bool pset_fwnode_property_present(const struct fwnode_handle *fwnode,
					 const char *propname)
{
	return !!pset_prop_get(to_pset_node(fwnode), propname);
}

static int pset_fwnode_read_int_array(const struct fwnode_handle *fwnode,
				      const char *propname,
				      unsigned int elem_size, void *val,
				      size_t nval)
{
	const struct property_set *node = to_pset_node(fwnode);

	if (!val)
		return pset_prop_count_elems_of_size(node, propname, elem_size);

	switch (elem_size) {
	case sizeof(u8):
		return pset_prop_read_u8_array(node, propname, val, nval);
	case sizeof(u16):
		return pset_prop_read_u16_array(node, propname, val, nval);
	case sizeof(u32):
		return pset_prop_read_u32_array(node, propname, val, nval);
	case sizeof(u64):
		return pset_prop_read_u64_array(node, propname, val, nval);
	}

	return -ENXIO;
}

static int
pset_fwnode_property_read_string_array(const struct fwnode_handle *fwnode,
				       const char *propname,
				       const char **val, size_t nval)
{
	return pset_prop_read_string_array(to_pset_node(fwnode), propname,
					   val, nval);
}

static const struct fwnode_operations pset_fwnode_ops = {
	.property_present = pset_fwnode_property_present,
	.property_read_int_array = pset_fwnode_read_int_array,
	.property_read_string_array = pset_fwnode_property_read_string_array,
};

/**
 * device_property_present - check if a property of a device is present
 * @dev: Device whose property is being checked
 * @propname: Name of the property
 *
 * Check if property @propname is present in the device firmware description.
 */
bool device_property_present(struct device *dev, const char *propname)
{
	return fwnode_property_present(dev_fwnode(dev), propname);
}
EXPORT_SYMBOL_GPL(device_property_present);

/**
 * fwnode_property_present - check if a property of a firmware node is present
 * @fwnode: Firmware node whose property to check
 * @propname: Name of the property
 */
bool fwnode_property_present(const struct fwnode_handle *fwnode,
			     const char *propname)
{
	bool ret;

	ret = fwnode_call_bool_op(fwnode, property_present, propname);
	if (ret == false && !IS_ERR_OR_NULL(fwnode) &&
	    !IS_ERR_OR_NULL(fwnode->secondary))
		ret = fwnode_call_bool_op(fwnode->secondary, property_present,
					 propname);
	return ret;
}
EXPORT_SYMBOL_GPL(fwnode_property_present);

/**
 * device_property_read_u8_array - return a u8 array property of a device
 * @dev: Device to get the property of
 * @propname: Name of the property
 * @val: The values are stored here or %NULL to return the number of values
 * @nval: Size of the @val array
 *
 * Function reads an array of u8 properties with @propname from the device
 * firmware description and stores them to @val if found.
 *
 * Return: number of values if @val was %NULL,
 *         %0 if the property was found (success),
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO if the property is not an array of numbers,
 *	   %-EOVERFLOW if the size of the property is not as expected.
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int device_property_read_u8_array(struct device *dev, const char *propname,
				  u8 *val, size_t nval)
{
	return fwnode_property_read_u8_array(dev_fwnode(dev), propname, val, nval);
}
EXPORT_SYMBOL_GPL(device_property_read_u8_array);

/**
 * device_property_read_u16_array - return a u16 array property of a device
 * @dev: Device to get the property of
 * @propname: Name of the property
 * @val: The values are stored here or %NULL to return the number of values
 * @nval: Size of the @val array
 *
 * Function reads an array of u16 properties with @propname from the device
 * firmware description and stores them to @val if found.
 *
 * Return: number of values if @val was %NULL,
 *         %0 if the property was found (success),
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO if the property is not an array of numbers,
 *	   %-EOVERFLOW if the size of the property is not as expected.
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int device_property_read_u16_array(struct device *dev, const char *propname,
				   u16 *val, size_t nval)
{
	return fwnode_property_read_u16_array(dev_fwnode(dev), propname, val, nval);
}
EXPORT_SYMBOL_GPL(device_property_read_u16_array);

/**
 * device_property_read_u32_array - return a u32 array property of a device
 * @dev: Device to get the property of
 * @propname: Name of the property
 * @val: The values are stored here or %NULL to return the number of values
 * @nval: Size of the @val array
 *
 * Function reads an array of u32 properties with @propname from the device
 * firmware description and stores them to @val if found.
 *
 * Return: number of values if @val was %NULL,
 *         %0 if the property was found (success),
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO if the property is not an array of numbers,
 *	   %-EOVERFLOW if the size of the property is not as expected.
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int device_property_read_u32_array(struct device *dev, const char *propname,
				   u32 *val, size_t nval)
{
	return fwnode_property_read_u32_array(dev_fwnode(dev), propname, val, nval);
}
EXPORT_SYMBOL_GPL(device_property_read_u32_array);

/**
 * device_property_read_u64_array - return a u64 array property of a device
 * @dev: Device to get the property of
 * @propname: Name of the property
 * @val: The values are stored here or %NULL to return the number of values
 * @nval: Size of the @val array
 *
 * Function reads an array of u64 properties with @propname from the device
 * firmware description and stores them to @val if found.
 *
 * Return: number of values if @val was %NULL,
 *         %0 if the property was found (success),
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO if the property is not an array of numbers,
 *	   %-EOVERFLOW if the size of the property is not as expected.
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int device_property_read_u64_array(struct device *dev, const char *propname,
				   u64 *val, size_t nval)
{
	return fwnode_property_read_u64_array(dev_fwnode(dev), propname, val, nval);
}
EXPORT_SYMBOL_GPL(device_property_read_u64_array);

/**
 * device_property_read_string_array - return a string array property of device
 * @dev: Device to get the property of
 * @propname: Name of the property
 * @val: The values are stored here or %NULL to return the number of values
 * @nval: Size of the @val array
 *
 * Function reads an array of string properties with @propname from the device
 * firmware description and stores them to @val if found.
 *
 * Return: number of values read on success if @val is non-NULL,
 *	   number of values available on success if @val is NULL,
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO or %-EILSEQ if the property is not an array of strings,
 *	   %-EOVERFLOW if the size of the property is not as expected.
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int device_property_read_string_array(struct device *dev, const char *propname,
				      const char **val, size_t nval)
{
	return fwnode_property_read_string_array(dev_fwnode(dev), propname, val, nval);
}
EXPORT_SYMBOL_GPL(device_property_read_string_array);

/**
 * device_property_read_string - return a string property of a device
 * @dev: Device to get the property of
 * @propname: Name of the property
 * @val: The value is stored here
 *
 * Function reads property @propname from the device firmware description and
 * stores the value into @val if found. The value is checked to be a string.
 *
 * Return: %0 if the property was found (success),
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO or %-EILSEQ if the property type is not a string.
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int device_property_read_string(struct device *dev, const char *propname,
				const char **val)
{
	return fwnode_property_read_string(dev_fwnode(dev), propname, val);
}
EXPORT_SYMBOL_GPL(device_property_read_string);

/**
 * device_property_match_string - find a string in an array and return index
 * @dev: Device to get the property of
 * @propname: Name of the property holding the array
 * @string: String to look for
 *
 * Find a given string in a string array and if it is found return the
 * index back.
 *
 * Return: %0 if the property was found (success),
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO if the property is not an array of strings,
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int device_property_match_string(struct device *dev, const char *propname,
				 const char *string)
{
	return fwnode_property_match_string(dev_fwnode(dev), propname, string);
}
EXPORT_SYMBOL_GPL(device_property_match_string);

static int fwnode_property_read_int_array(const struct fwnode_handle *fwnode,
					  const char *propname,
					  unsigned int elem_size, void *val,
					  size_t nval)
{
	int ret;

	ret = fwnode_call_int_op(fwnode, property_read_int_array, propname,
				 elem_size, val, nval);
	if (ret == -EINVAL && !IS_ERR_OR_NULL(fwnode) &&
	    !IS_ERR_OR_NULL(fwnode->secondary))
		ret = fwnode_call_int_op(
			fwnode->secondary, property_read_int_array, propname,
			elem_size, val, nval);

	return ret;
}

/**
 * fwnode_property_read_u8_array - return a u8 array property of firmware node
 * @fwnode: Firmware node to get the property of
 * @propname: Name of the property
 * @val: The values are stored here or %NULL to return the number of values
 * @nval: Size of the @val array
 *
 * Read an array of u8 properties with @propname from @fwnode and stores them to
 * @val if found.
 *
 * Return: number of values if @val was %NULL,
 *         %0 if the property was found (success),
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO if the property is not an array of numbers,
 *	   %-EOVERFLOW if the size of the property is not as expected,
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int fwnode_property_read_u8_array(const struct fwnode_handle *fwnode,
				  const char *propname, u8 *val, size_t nval)
{
	return fwnode_property_read_int_array(fwnode, propname, sizeof(u8),
					      val, nval);
}
EXPORT_SYMBOL_GPL(fwnode_property_read_u8_array);

/**
 * fwnode_property_read_u16_array - return a u16 array property of firmware node
 * @fwnode: Firmware node to get the property of
 * @propname: Name of the property
 * @val: The values are stored here or %NULL to return the number of values
 * @nval: Size of the @val array
 *
 * Read an array of u16 properties with @propname from @fwnode and store them to
 * @val if found.
 *
 * Return: number of values if @val was %NULL,
 *         %0 if the property was found (success),
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO if the property is not an array of numbers,
 *	   %-EOVERFLOW if the size of the property is not as expected,
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int fwnode_property_read_u16_array(const struct fwnode_handle *fwnode,
				   const char *propname, u16 *val, size_t nval)
{
	return fwnode_property_read_int_array(fwnode, propname, sizeof(u16),
					      val, nval);
}
EXPORT_SYMBOL_GPL(fwnode_property_read_u16_array);

/**
 * fwnode_property_read_u32_array - return a u32 array property of firmware node
 * @fwnode: Firmware node to get the property of
 * @propname: Name of the property
 * @val: The values are stored here or %NULL to return the number of values
 * @nval: Size of the @val array
 *
 * Read an array of u32 properties with @propname from @fwnode store them to
 * @val if found.
 *
 * Return: number of values if @val was %NULL,
 *         %0 if the property was found (success),
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO if the property is not an array of numbers,
 *	   %-EOVERFLOW if the size of the property is not as expected,
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int fwnode_property_read_u32_array(const struct fwnode_handle *fwnode,
				   const char *propname, u32 *val, size_t nval)
{
	return fwnode_property_read_int_array(fwnode, propname, sizeof(u32),
					      val, nval);
}
EXPORT_SYMBOL_GPL(fwnode_property_read_u32_array);

/**
 * fwnode_property_read_u64_array - return a u64 array property firmware node
 * @fwnode: Firmware node to get the property of
 * @propname: Name of the property
 * @val: The values are stored here or %NULL to return the number of values
 * @nval: Size of the @val array
 *
 * Read an array of u64 properties with @propname from @fwnode and store them to
 * @val if found.
 *
 * Return: number of values if @val was %NULL,
 *         %0 if the property was found (success),
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO if the property is not an array of numbers,
 *	   %-EOVERFLOW if the size of the property is not as expected,
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int fwnode_property_read_u64_array(const struct fwnode_handle *fwnode,
				   const char *propname, u64 *val, size_t nval)
{
	return fwnode_property_read_int_array(fwnode, propname, sizeof(u64),
					      val, nval);
}
EXPORT_SYMBOL_GPL(fwnode_property_read_u64_array);

/**
 * fwnode_property_read_string_array - return string array property of a node
 * @fwnode: Firmware node to get the property of
 * @propname: Name of the property
 * @val: The values are stored here or %NULL to return the number of values
 * @nval: Size of the @val array
 *
 * Read an string list property @propname from the given firmware node and store
 * them to @val if found.
 *
 * Return: number of values read on success if @val is non-NULL,
 *	   number of values available on success if @val is NULL,
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO or %-EILSEQ if the property is not an array of strings,
 *	   %-EOVERFLOW if the size of the property is not as expected,
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int fwnode_property_read_string_array(const struct fwnode_handle *fwnode,
				      const char *propname, const char **val,
				      size_t nval)
{
	int ret;

	ret = fwnode_call_int_op(fwnode, property_read_string_array, propname,
				 val, nval);
	if (ret == -EINVAL && !IS_ERR_OR_NULL(fwnode) &&
	    !IS_ERR_OR_NULL(fwnode->secondary))
		ret = fwnode_call_int_op(fwnode->secondary,
					 property_read_string_array, propname,
					 val, nval);
	return ret;
}
EXPORT_SYMBOL_GPL(fwnode_property_read_string_array);

/**
 * fwnode_property_read_string - return a string property of a firmware node
 * @fwnode: Firmware node to get the property of
 * @propname: Name of the property
 * @val: The value is stored here
 *
 * Read property @propname from the given firmware node and store the value into
 * @val if found.  The value is checked to be a string.
 *
 * Return: %0 if the property was found (success),
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO or %-EILSEQ if the property is not a string,
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int fwnode_property_read_string(const struct fwnode_handle *fwnode,
				const char *propname, const char **val)
{
	int ret = fwnode_property_read_string_array(fwnode, propname, val, 1);

	return ret < 0 ? ret : 0;
}
EXPORT_SYMBOL_GPL(fwnode_property_read_string);

/**
 * fwnode_property_match_string - find a string in an array and return index
 * @fwnode: Firmware node to get the property of
 * @propname: Name of the property holding the array
 * @string: String to look for
 *
 * Find a given string in a string array and if it is found return the
 * index back.
 *
 * Return: %0 if the property was found (success),
 *	   %-EINVAL if given arguments are not valid,
 *	   %-ENODATA if the property does not have a value,
 *	   %-EPROTO if the property is not an array of strings,
 *	   %-ENXIO if no suitable firmware interface is present.
 */
int fwnode_property_match_string(const struct fwnode_handle *fwnode,
	const char *propname, const char *string)
{
	const char **values;
	int nval, ret;

	nval = fwnode_property_read_string_array(fwnode, propname, NULL, 0);
	if (nval < 0)
		return nval;

	if (nval == 0)
		return -ENODATA;

	values = kcalloc(nval, sizeof(*values), GFP_KERNEL);
	if (!values)
		return -ENOMEM;

	ret = fwnode_property_read_string_array(fwnode, propname, values, nval);
	if (ret < 0)
		goto out;

	ret = match_string(values, nval, string);
	if (ret < 0)
		ret = -ENODATA;
out:
	kfree(values);
	return ret;
}
EXPORT_SYMBOL_GPL(fwnode_property_match_string);

/**
 * fwnode_property_get_reference_args() - Find a reference with arguments
 * @fwnode:	Firmware node where to look for the reference
 * @prop:	The name of the property
 * @nargs_prop:	The name of the property telling the number of
 *		arguments in the referred node. NULL if @nargs is known,
 *		otherwise @nargs is ignored. Only relevant on OF.
 * @nargs:	Number of arguments. Ignored if @nargs_prop is non-NULL.
 * @index:	Index of the reference, from zero onwards.
 * @args:	Result structure with reference and integer arguments.
 *
 * Obtain a reference based on a named property in an fwnode, with
 * integer arguments.
 *
 * Caller is responsible to call fwnode_handle_put() on the returned
 * args->fwnode pointer.
 *
 * Returns: %0 on success
 *	    %-ENOENT when the index is out of bounds, the index has an empty
 *		     reference or the property was not found
 *	    %-EINVAL on parse error
 */
int fwnode_property_get_reference_args(const struct fwnode_handle *fwnode,
				       const char *prop, const char *nargs_prop,
				       unsigned int nargs, unsigned int index,
				       struct fwnode_reference_args *args)
{
	return fwnode_call_int_op(fwnode, get_reference_args, prop, nargs_prop,
				  nargs, index, args);
}
EXPORT_SYMBOL_GPL(fwnode_property_get_reference_args);

static void property_entry_free_data(const struct property_entry *p)
{
	const void *pointer = property_get_pointer(p);
	size_t i, nval;

	if (p->is_array) {
		if (p->type == DEV_PROP_STRING && p->pointer.str) {
			nval = p->length / sizeof(const char *);
			for (i = 0; i < nval; i++)
				kfree(p->pointer.str[i]);
		}
		kfree(pointer);
	} else if (p->type == DEV_PROP_STRING) {
		kfree(p->value.str);
	}
	kfree(p->name);
}

static int property_copy_string_array(struct property_entry *dst,
				      const struct property_entry *src)
{
	const char **d;
	size_t nval = src->length / sizeof(*d);
	int i;

	d = kcalloc(nval, sizeof(*d), GFP_KERNEL);
	if (!d)
		return -ENOMEM;

	for (i = 0; i < nval; i++) {
		d[i] = kstrdup(src->pointer.str[i], GFP_KERNEL);
		if (!d[i] && src->pointer.str[i]) {
			while (--i >= 0)
				kfree(d[i]);
			kfree(d);
			return -ENOMEM;
		}
	}

	dst->pointer.str = d;
	return 0;
}

static int property_entry_copy_data(struct property_entry *dst,
				    const struct property_entry *src)
{
	const void *pointer = property_get_pointer(src);
	const void *new;
	int error;

	if (src->is_array) {
		if (!src->length)
			return -ENODATA;

		if (src->type == DEV_PROP_STRING) {
			error = property_copy_string_array(dst, src);
			if (error)
				return error;
			new = dst->pointer.str;
		} else {
			new = kmemdup(pointer, src->length, GFP_KERNEL);
			if (!new)
				return -ENOMEM;
		}
	} else if (src->type == DEV_PROP_STRING) {
		new = kstrdup(src->value.str, GFP_KERNEL);
		if (!new && src->value.str)
			return -ENOMEM;
	} else {
		new = pointer;
	}

	dst->length = src->length;
	dst->is_array = src->is_array;
	dst->type = src->type;

	property_set_pointer(dst, new);

	dst->name = kstrdup(src->name, GFP_KERNEL);
	if (!dst->name)
		goto out_free_data;

	return 0;

out_free_data:
	property_entry_free_data(dst);
	return -ENOMEM;
}

/**
 * property_entries_dup - duplicate array of properties
 * @properties: array of properties to copy
 *
 * This function creates a deep copy of the given NULL-terminated array
 * of property entries.
 */
struct property_entry *
property_entries_dup(const struct property_entry *properties)
{
	struct property_entry *p;
	int i, n = 0;

	while (properties[n].name)
		n++;

	p = kcalloc(n + 1, sizeof(*p), GFP_KERNEL);
	if (!p)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < n; i++) {
		int ret = property_entry_copy_data(&p[i], &properties[i]);
		if (ret) {
			while (--i >= 0)
				property_entry_free_data(&p[i]);
			kfree(p);
			return ERR_PTR(ret);
		}
	}

	return p;
}
EXPORT_SYMBOL_GPL(property_entries_dup);

/**
 * property_entries_free - free previously allocated array of properties
 * @properties: array of properties to destroy
 *
 * This function frees given NULL-terminated array of property entries,
 * along with their data.
 */
void property_entries_free(const struct property_entry *properties)
{
	const struct property_entry *p;

	for (p = properties; p->name; p++)
		property_entry_free_data(p);

	kfree(properties);
}
EXPORT_SYMBOL_GPL(property_entries_free);

/**
 * pset_free_set - releases memory allocated for copied property set
 * @pset: Property set to release
 *
 * Function takes previously copied property set and releases all the
 * memory allocated to it.
 */
static void pset_free_set(struct property_set *pset)
{
	if (!pset)
		return;

	property_entries_free(pset->properties);
	kfree(pset);
}

/**
 * pset_copy_set - copies property set
 * @pset: Property set to copy
 *
 * This function takes a deep copy of the given property set and returns
 * pointer to the copy. Call device_free_property_set() to free resources
 * allocated in this function.
 *
 * Return: Pointer to the new property set or error pointer.
 */
static struct property_set *pset_copy_set(const struct property_set *pset)
{
	struct property_entry *properties;
	struct property_set *p;

	p = kzalloc(sizeof(*p), GFP_KERNEL);
	if (!p)
		return ERR_PTR(-ENOMEM);

	properties = property_entries_dup(pset->properties);
	if (IS_ERR(properties)) {
		kfree(p);
		return ERR_CAST(properties);
	}

	p->properties = properties;
	return p;
}

/**
 * device_remove_properties - Remove properties from a device object.
 * @dev: Device whose properties to remove.
 *
 * The function removes properties previously associated to the device
 * secondary firmware node with device_add_properties(). Memory allocated
 * to the properties will also be released.
 */
void device_remove_properties(struct device *dev)
{
	struct fwnode_handle *fwnode;
	struct property_set *pset;

	fwnode = dev_fwnode(dev);
	if (!fwnode)
		return;
	/*
	 * Pick either primary or secondary node depending which one holds
	 * the pset. If there is no real firmware node (ACPI/DT) primary
	 * will hold the pset.
	 */
	pset = to_pset_node(fwnode);
	if (pset) {
		set_primary_fwnode(dev, NULL);
	} else {
		pset = to_pset_node(fwnode->secondary);
		if (pset && dev == pset->dev)
			set_secondary_fwnode(dev, NULL);
	}
	if (pset && dev == pset->dev)
		pset_free_set(pset);
}
EXPORT_SYMBOL_GPL(device_remove_properties);

/**
 * device_add_properties - Add a collection of properties to a device object.
 * @dev: Device to add properties to.
 * @properties: Collection of properties to add.
 *
 * Associate a collection of device properties represented by @properties with
 * @dev as its secondary firmware node. The function takes a copy of
 * @properties.
 */
int device_add_properties(struct device *dev,
			  const struct property_entry *properties)
{
	struct property_set *p, pset;

	if (!properties)
		return -EINVAL;

	pset.properties = properties;

	p = pset_copy_set(&pset);
	if (IS_ERR(p))
		return PTR_ERR(p);

	p->fwnode.ops = &pset_fwnode_ops;
	set_secondary_fwnode(dev, &p->fwnode);
	p->dev = dev;
	return 0;
}
EXPORT_SYMBOL_GPL(device_add_properties);

/**
 * fwnode_get_next_parent - Iterate to the node's parent
 * @fwnode: Firmware whose parent is retrieved
 *
 * This is like fwnode_get_parent() except that it drops the refcount
 * on the passed node, making it suitable for iterating through a
 * node's parents.
 *
 * Returns a node pointer with refcount incremented, use
 * fwnode_handle_node() on it when done.
 */
struct fwnode_handle *fwnode_get_next_parent(struct fwnode_handle *fwnode)
{
	struct fwnode_handle *parent = fwnode_get_parent(fwnode);

	fwnode_handle_put(fwnode);

	return parent;
}
EXPORT_SYMBOL_GPL(fwnode_get_next_parent);

/**
 * fwnode_get_parent - Return parent firwmare node
 * @fwnode: Firmware whose parent is retrieved
 *
 * Return parent firmware node of the given node if possible or %NULL if no
 * parent was available.
 */
struct fwnode_handle *fwnode_get_parent(const struct fwnode_handle *fwnode)
{
	return fwnode_call_ptr_op(fwnode, get_parent);
}
EXPORT_SYMBOL_GPL(fwnode_get_parent);

/**
 * fwnode_get_next_child_node - Return the next child node handle for a node
 * @fwnode: Firmware node to find the next child node for.
 * @child: Handle to one of the node's child nodes or a %NULL handle.
 */
struct fwnode_handle *
fwnode_get_next_child_node(const struct fwnode_handle *fwnode,
			   struct fwnode_handle *child)
{
	return fwnode_call_ptr_op(fwnode, get_next_child_node, child);
}
EXPORT_SYMBOL_GPL(fwnode_get_next_child_node);

/**
 * fwnode_get_next_available_child_node - Return the next
 * available child node handle for a node
 * @fwnode: Firmware node to find the next child node for.
 * @child: Handle to one of the node's child nodes or a %NULL handle.
 */
struct fwnode_handle *
fwnode_get_next_available_child_node(const struct fwnode_handle *fwnode,
				     struct fwnode_handle *child)
{
	struct fwnode_handle *next_child = child;

	if (!fwnode)
		return NULL;

	do {
		next_child = fwnode_get_next_child_node(fwnode, next_child);

		if (!next_child || fwnode_device_is_available(next_child))
			break;
	} while (next_child);

	return next_child;
}
EXPORT_SYMBOL_GPL(fwnode_get_next_available_child_node);

/**
 * device_get_next_child_node - Return the next child node handle for a device
 * @dev: Device to find the next child node for.
 * @child: Handle to one of the device's child nodes or a null handle.
 */
struct fwnode_handle *device_get_next_child_node(struct device *dev,
						 struct fwnode_handle *child)
{
	struct acpi_device *adev = ACPI_COMPANION(dev);
	struct fwnode_handle *fwnode = NULL;

	if (dev->of_node)
		fwnode = &dev->of_node->fwnode;
	else if (adev)
		fwnode = acpi_fwnode_handle(adev);

	return fwnode_get_next_child_node(fwnode, child);
}
EXPORT_SYMBOL_GPL(device_get_next_child_node);

/**
 * fwnode_get_named_child_node - Return first matching named child node handle
 * @fwnode: Firmware node to find the named child node for.
 * @childname: String to match child node name against.
 */
struct fwnode_handle *
fwnode_get_named_child_node(const struct fwnode_handle *fwnode,
			    const char *childname)
{
	return fwnode_call_ptr_op(fwnode, get_named_child_node, childname);
}
EXPORT_SYMBOL_GPL(fwnode_get_named_child_node);

/**
 * device_get_named_child_node - Return first matching named child node handle
 * @dev: Device to find the named child node for.
 * @childname: String to match child node name against.
 */
struct fwnode_handle *device_get_named_child_node(struct device *dev,
						  const char *childname)
{
	return fwnode_get_named_child_node(dev_fwnode(dev), childname);
}
EXPORT_SYMBOL_GPL(device_get_named_child_node);

/**
 * fwnode_handle_get - Obtain a reference to a device node
 * @fwnode: Pointer to the device node to obtain the reference to.
 *
 * Returns the fwnode handle.
 */
struct fwnode_handle *fwnode_handle_get(struct fwnode_handle *fwnode)
{
	if (!fwnode_has_op(fwnode, get))
		return fwnode;

	return fwnode_call_ptr_op(fwnode, get);
}
EXPORT_SYMBOL_GPL(fwnode_handle_get);

/**
 * fwnode_handle_put - Drop reference to a device node
 * @fwnode: Pointer to the device node to drop the reference to.
 *
 * This has to be used when terminating device_for_each_child_node() iteration
 * with break or return to prevent stale device node references from being left
 * behind.
 */
void fwnode_handle_put(struct fwnode_handle *fwnode)
{
	fwnode_call_void_op(fwnode, put);
}
EXPORT_SYMBOL_GPL(fwnode_handle_put);

/**
 * fwnode_device_is_available - check if a device is available for use
 * @fwnode: Pointer to the fwnode of the device.
 */
bool fwnode_device_is_available(const struct fwnode_handle *fwnode)
{
	return fwnode_call_bool_op(fwnode, device_is_available);
}
EXPORT_SYMBOL_GPL(fwnode_device_is_available);

/**
 * device_get_child_node_count - return the number of child nodes for device
 * @dev: Device to cound the child nodes for
 */
unsigned int device_get_child_node_count(struct device *dev)
{
	struct fwnode_handle *child;
	unsigned int count = 0;

	device_for_each_child_node(dev, child)
		count++;

	return count;
}
EXPORT_SYMBOL_GPL(device_get_child_node_count);

bool device_dma_supported(struct device *dev)
{
	/* For DT, this is always supported.
	 * For ACPI, this depends on CCA, which
	 * is determined by the acpi_dma_supported().
	 */
	if (IS_ENABLED(CONFIG_OF) && dev->of_node)
		return true;

	return acpi_dma_supported(ACPI_COMPANION(dev));
}
EXPORT_SYMBOL_GPL(device_dma_supported);

enum dev_dma_attr device_get_dma_attr(struct device *dev)
{
	enum dev_dma_attr attr = DEV_DMA_NOT_SUPPORTED;

	if (IS_ENABLED(CONFIG_OF) && dev->of_node) {
		if (of_dma_is_coherent(dev->of_node))
			attr = DEV_DMA_COHERENT;
		else
			attr = DEV_DMA_NON_COHERENT;
	} else
		attr = acpi_get_dma_attr(ACPI_COMPANION(dev));

	return attr;
}
EXPORT_SYMBOL_GPL(device_get_dma_attr);

/**
 * fwnode_get_phy_mode - Get phy mode for given firmware node
 * @fwnode:	Pointer to the given node
 *
 * The function gets phy interface string from property 'phy-mode' or
 * 'phy-connection-type', and return its index in phy_modes table, or errno in
 * error case.
 */
int fwnode_get_phy_mode(struct fwnode_handle *fwnode)
{
	const char *pm;
	int err, i;

	err = fwnode_property_read_string(fwnode, "phy-mode", &pm);
	if (err < 0)
		err = fwnode_property_read_string(fwnode,
						  "phy-connection-type", &pm);
	if (err < 0)
		return err;

	for (i = 0; i < PHY_INTERFACE_MODE_MAX; i++)
		if (!strcasecmp(pm, phy_modes(i)))
			return i;

	return -ENODEV;
}
EXPORT_SYMBOL_GPL(fwnode_get_phy_mode);

/**
 * device_get_phy_mode - Get phy mode for given device
 * @dev:	Pointer to the given device
 *
 * The function gets phy interface string from property 'phy-mode' or
 * 'phy-connection-type', and return its index in phy_modes table, or errno in
 * error case.
 */
int device_get_phy_mode(struct device *dev)
{
	return fwnode_get_phy_mode(dev_fwnode(dev));
}
EXPORT_SYMBOL_GPL(device_get_phy_mode);

static void *fwnode_get_mac_addr(struct fwnode_handle *fwnode,
				 const char *name, char *addr,
				 int alen)
{
	int ret = fwnode_property_read_u8_array(fwnode, name, addr, alen);

	if (ret == 0 && alen == ETH_ALEN && is_valid_ether_addr(addr))
		return addr;
	return NULL;
}

/**
 * fwnode_get_mac_address - Get the MAC from the firmware node
 * @fwnode:	Pointer to the firmware node
 * @addr:	Address of buffer to store the MAC in
 * @alen:	Length of the buffer pointed to by addr, should be ETH_ALEN
 *
 * Search the firmware node for the best MAC address to use.  'mac-address' is
 * checked first, because that is supposed to contain to "most recent" MAC
 * address. If that isn't set, then 'local-mac-address' is checked next,
 * because that is the default address.  If that isn't set, then the obsolete
 * 'address' is checked, just in case we're using an old device tree.
 *
 * Note that the 'address' property is supposed to contain a virtual address of
 * the register set, but some DTS files have redefined that property to be the
 * MAC address.
 *
 * All-zero MAC addresses are rejected, because those could be properties that
 * exist in the firmware tables, but were not updated by the firmware.  For
 * example, the DTS could define 'mac-address' and 'local-mac-address', with
 * zero MAC addresses.  Some older U-Boots only initialized 'local-mac-address'.
 * In this case, the real MAC is in 'local-mac-address', and 'mac-address'
 * exists but is all zeros.
*/
void *fwnode_get_mac_address(struct fwnode_handle *fwnode, char *addr, int alen)
{
	char *res;

	res = fwnode_get_mac_addr(fwnode, "mac-address", addr, alen);
	if (res)
		return res;

	res = fwnode_get_mac_addr(fwnode, "local-mac-address", addr, alen);
	if (res)
		return res;

	return fwnode_get_mac_addr(fwnode, "address", addr, alen);
}
EXPORT_SYMBOL(fwnode_get_mac_address);

/**
 * device_get_mac_address - Get the MAC for a given device
 * @dev:	Pointer to the device
 * @addr:	Address of buffer to store the MAC in
 * @alen:	Length of the buffer pointed to by addr, should be ETH_ALEN
 */
void *device_get_mac_address(struct device *dev, char *addr, int alen)
{
	return fwnode_get_mac_address(dev_fwnode(dev), addr, alen);
}
EXPORT_SYMBOL(device_get_mac_address);

/**
 * fwnode_irq_get - Get IRQ directly from a fwnode
 * @fwnode:	Pointer to the firmware node
 * @index:	Zero-based index of the IRQ
 *
 * Returns Linux IRQ number on success. Other values are determined
 * accordingly to acpi_/of_ irq_get() operation.
 */
int fwnode_irq_get(struct fwnode_handle *fwnode, unsigned int index)
{
	struct device_node *of_node = to_of_node(fwnode);
	struct resource res;
	int ret;

	if (IS_ENABLED(CONFIG_OF) && of_node)
		return of_irq_get(of_node, index);

	ret = acpi_irq_get(ACPI_HANDLE_FWNODE(fwnode), index, &res);
	if (ret)
		return ret;

	return res.start;
}
EXPORT_SYMBOL(fwnode_irq_get);

/**
 * device_graph_get_next_endpoint - Get next endpoint firmware node
 * @fwnode: Pointer to the parent firmware node
 * @prev: Previous endpoint node or %NULL to get the first
 *
 * Returns an endpoint firmware node pointer or %NULL if no more endpoints
 * are available.
 */
struct fwnode_handle *
fwnode_graph_get_next_endpoint(const struct fwnode_handle *fwnode,
			       struct fwnode_handle *prev)
{
	return fwnode_call_ptr_op(fwnode, graph_get_next_endpoint, prev);
}
EXPORT_SYMBOL_GPL(fwnode_graph_get_next_endpoint);

/**
 * fwnode_graph_get_port_parent - Return the device fwnode of a port endpoint
 * @endpoint: Endpoint firmware node of the port
 *
 * Return: the firmware node of the device the @endpoint belongs to.
 */
struct fwnode_handle *
fwnode_graph_get_port_parent(const struct fwnode_handle *endpoint)
{
	struct fwnode_handle *port, *parent;

	port = fwnode_get_parent(endpoint);
	parent = fwnode_call_ptr_op(port, graph_get_port_parent);

	fwnode_handle_put(port);

	return parent;
}
EXPORT_SYMBOL_GPL(fwnode_graph_get_port_parent);

/**
 * fwnode_graph_get_remote_port_parent - Return fwnode of a remote device
 * @fwnode: Endpoint firmware node pointing to the remote endpoint
 *
 * Extracts firmware node of a remote device the @fwnode points to.
 */
struct fwnode_handle *
fwnode_graph_get_remote_port_parent(const struct fwnode_handle *fwnode)
{
	struct fwnode_handle *endpoint, *parent;

	endpoint = fwnode_graph_get_remote_endpoint(fwnode);
	parent = fwnode_graph_get_port_parent(endpoint);

	fwnode_handle_put(endpoint);

	return parent;
}
EXPORT_SYMBOL_GPL(fwnode_graph_get_remote_port_parent);

/**
 * fwnode_graph_get_remote_port - Return fwnode of a remote port
 * @fwnode: Endpoint firmware node pointing to the remote endpoint
 *
 * Extracts firmware node of a remote port the @fwnode points to.
 */
struct fwnode_handle *
fwnode_graph_get_remote_port(const struct fwnode_handle *fwnode)
{
	return fwnode_get_next_parent(fwnode_graph_get_remote_endpoint(fwnode));
}
EXPORT_SYMBOL_GPL(fwnode_graph_get_remote_port);

/**
 * fwnode_graph_get_remote_endpoint - Return fwnode of a remote endpoint
 * @fwnode: Endpoint firmware node pointing to the remote endpoint
 *
 * Extracts firmware node of a remote endpoint the @fwnode points to.
 */
struct fwnode_handle *
fwnode_graph_get_remote_endpoint(const struct fwnode_handle *fwnode)
{
	return fwnode_call_ptr_op(fwnode, graph_get_remote_endpoint);
}
EXPORT_SYMBOL_GPL(fwnode_graph_get_remote_endpoint);

/**
 * fwnode_graph_get_remote_node - get remote parent node for given port/endpoint
 * @fwnode: pointer to parent fwnode_handle containing graph port/endpoint
 * @port_id: identifier of the parent port node
 * @endpoint_id: identifier of the endpoint node
 *
 * Return: Remote fwnode handle associated with remote endpoint node linked
 *	   to @node. Use fwnode_node_put() on it when done.
 */
struct fwnode_handle *
fwnode_graph_get_remote_node(const struct fwnode_handle *fwnode, u32 port_id,
			     u32 endpoint_id)
{
	struct fwnode_handle *endpoint = NULL;

	while ((endpoint = fwnode_graph_get_next_endpoint(fwnode, endpoint))) {
		struct fwnode_endpoint fwnode_ep;
		struct fwnode_handle *remote;
		int ret;

		ret = fwnode_graph_parse_endpoint(endpoint, &fwnode_ep);
		if (ret < 0)
			continue;

		if (fwnode_ep.port != port_id || fwnode_ep.id != endpoint_id)
			continue;

		remote = fwnode_graph_get_remote_port_parent(endpoint);
		if (!remote)
			return NULL;

		return fwnode_device_is_available(remote) ? remote : NULL;
	}

	return NULL;
}
EXPORT_SYMBOL_GPL(fwnode_graph_get_remote_node);

/**
 * fwnode_graph_parse_endpoint - parse common endpoint node properties
 * @fwnode: pointer to endpoint fwnode_handle
 * @endpoint: pointer to the fwnode endpoint data structure
 *
 * Parse @fwnode representing a graph endpoint node and store the
 * information in @endpoint. The caller must hold a reference to
 * @fwnode.
 */
int fwnode_graph_parse_endpoint(const struct fwnode_handle *fwnode,
				struct fwnode_endpoint *endpoint)
{
	memset(endpoint, 0, sizeof(*endpoint));

	return fwnode_call_int_op(fwnode, graph_parse_endpoint, endpoint);
}
EXPORT_SYMBOL(fwnode_graph_parse_endpoint);

const void *device_get_match_data(struct device *dev)
{
	return fwnode_call_ptr_op(dev_fwnode(dev), device_get_match_data, dev);
}
EXPORT_SYMBOL_GPL(device_get_match_data);
