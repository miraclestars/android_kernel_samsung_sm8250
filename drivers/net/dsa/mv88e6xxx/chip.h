/*
 * Marvell 88E6xxx Ethernet switch single-chip definition
 *
 * Copyright (c) 2008 Marvell Semiconductor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef _MV88E6XXX_CHIP_H
#define _MV88E6XXX_CHIP_H

#include <linux/if_vlan.h>
#include <linux/irq.h>
#include <linux/gpio/consumer.h>
#include <linux/kthread.h>
#include <linux/phy.h>
#include <linux/ptp_clock_kernel.h>
#include <linux/timecounter.h>
#include <net/dsa.h>

#define SMI_CMD			0x00
#define SMI_CMD_BUSY		BIT(15)
#define SMI_CMD_CLAUSE_22	BIT(12)
#define SMI_CMD_OP_22_WRITE	((1 << 10) | SMI_CMD_BUSY | SMI_CMD_CLAUSE_22)
#define SMI_CMD_OP_22_READ	((2 << 10) | SMI_CMD_BUSY | SMI_CMD_CLAUSE_22)
#define SMI_CMD_OP_45_WRITE_ADDR	((0 << 10) | SMI_CMD_BUSY)
#define SMI_CMD_OP_45_WRITE_DATA	((1 << 10) | SMI_CMD_BUSY)
#define SMI_CMD_OP_45_READ_DATA		((2 << 10) | SMI_CMD_BUSY)
#define SMI_CMD_OP_45_READ_DATA_INC	((3 << 10) | SMI_CMD_BUSY)
#define SMI_DATA		0x01

#define MV88E6XXX_N_FID		4096

/* PVT limits for 4-bit port and 5-bit switch */
#define MV88E6XXX_MAX_PVT_SWITCHES	32
#define MV88E6XXX_MAX_PVT_PORTS		16

#define MV88E6XXX_MAX_GPIO	16

enum mv88e6xxx_egress_mode {
	MV88E6XXX_EGRESS_MODE_UNMODIFIED,
	MV88E6XXX_EGRESS_MODE_UNTAGGED,
	MV88E6XXX_EGRESS_MODE_TAGGED,
	MV88E6XXX_EGRESS_MODE_ETHERTYPE,
};

enum mv88e6xxx_frame_mode {
	MV88E6XXX_FRAME_MODE_NORMAL,
	MV88E6XXX_FRAME_MODE_DSA,
	MV88E6XXX_FRAME_MODE_PROVIDER,
	MV88E6XXX_FRAME_MODE_ETHERTYPE,
};

/* List of supported models */
enum mv88e6xxx_model {
	MV88E6085,
	MV88E6095,
	MV88E6097,
	MV88E6123,
	MV88E6131,
	MV88E6141,
	MV88E6161,
	MV88E6165,
	MV88E6171,
	MV88E6172,
	MV88E6175,
	MV88E6176,
	MV88E6185,
	MV88E6190,
	MV88E6190X,
	MV88E6191,
	MV88E6240,
	MV88E6290,
	MV88E6320,
	MV88E6321,
	MV88E6341,
	MV88E6350,
	MV88E6351,
	MV88E6352,
	MV88E6390,
	MV88E6390X,
};

enum mv88e6xxx_family {
	MV88E6XXX_FAMILY_NONE,
	MV88E6XXX_FAMILY_6065,	/* 6031 6035 6061 6065 */
	MV88E6XXX_FAMILY_6095,	/* 6092 6095 */
	MV88E6XXX_FAMILY_6097,	/* 6046 6085 6096 6097 */
	MV88E6XXX_FAMILY_6165,	/* 6123 6161 6165 */
	MV88E6XXX_FAMILY_6185,	/* 6108 6121 6122 6131 6152 6155 6182 6185 */
	MV88E6XXX_FAMILY_6320,	/* 6320 6321 */
	MV88E6XXX_FAMILY_6341,	/* 6141 6341 */
	MV88E6XXX_FAMILY_6351,	/* 6171 6175 6350 6351 */
	MV88E6XXX_FAMILY_6352,	/* 6172 6176 6240 6352 */
	MV88E6XXX_FAMILY_6390,  /* 6190 6190X 6191 6290 6390 6390X */
};

struct mv88e6xxx_ops;

struct mv88e6xxx_info {
	enum mv88e6xxx_family family;
	u16 prod_num;
	const char *name;
	unsigned int num_databases;
	unsigned int num_ports;
	unsigned int num_internal_phys;
	unsigned int num_gpio;
	unsigned int max_vid;
	unsigned int port_base_addr;
	unsigned int phy_base_addr;
	unsigned int global1_addr;
	unsigned int global2_addr;
	unsigned int age_time_coeff;
	unsigned int g1_irqs;
	unsigned int g2_irqs;
	bool pvt;

	/* Multi-chip Addressing Mode.
	 * Some chips respond to only 2 registers of its own SMI device address
	 * when it is non-zero, and use indirect access to internal registers.
	 */
	bool multi_chip;
	enum dsa_tag_protocol tag_protocol;

	/* Mask for FromPort and ToPort value of PortVec used in ATU Move
	 * operation. 0 means that the ATU Move operation is not supported.
	 */
	u8 atu_move_port_mask;
	const struct mv88e6xxx_ops *ops;

	/* Supports PTP */
	bool ptp_support;
};

struct mv88e6xxx_atu_entry {
	u8	state;
	bool	trunk;
	u16	portvec;
	u8	mac[ETH_ALEN];
};

struct mv88e6xxx_vtu_entry {
	u16	vid;
	u16	fid;
	u8	sid;
	bool	valid;
	u8	member[DSA_MAX_PORTS];
	u8	state[DSA_MAX_PORTS];
};

struct mv88e6xxx_bus_ops;
struct mv88e6xxx_irq_ops;
struct mv88e6xxx_gpio_ops;
struct mv88e6xxx_avb_ops;
struct mv88e6xxx_ptp_ops;

struct mv88e6xxx_irq {
	u16 masked;
	struct irq_chip chip;
	struct irq_domain *domain;
	unsigned int nirqs;
};

/* state flags for mv88e6xxx_port_hwtstamp::state */
enum {
	MV88E6XXX_HWTSTAMP_ENABLED,
	MV88E6XXX_HWTSTAMP_TX_IN_PROGRESS,
};

struct mv88e6xxx_port_hwtstamp {
	/* Port index */
	int port_id;

	/* Timestamping state */
	unsigned long state;

	/* Resources for receive timestamping */
	struct sk_buff_head rx_queue;
	struct sk_buff_head rx_queue2;

	/* Resources for transmit timestamping */
	unsigned long tx_tstamp_start;
	struct sk_buff *tx_skb;
	u16 tx_seq_id;

	/* Current timestamp configuration */
	struct hwtstamp_config tstamp_config;
};

struct mv88e6xxx_port {
	struct mv88e6xxx_chip *chip;
	int port;
	u64 serdes_stats[2];
	u64 atu_member_violation;
	u64 atu_miss_violation;
	u64 atu_full_violation;
	u64 vtu_member_violation;
	u64 vtu_miss_violation;
	u8 cmode;
	int serdes_irq;
};

struct mv88e6xxx_chip {
	const struct mv88e6xxx_info *info;

	/* The dsa_switch this private structure is related to */
	struct dsa_switch *ds;

	/* The device this structure is associated to */
	struct device *dev;

	/* This mutex protects the access to the switch registers */
	struct mutex reg_lock;

	/* The MII bus and the address on the bus that is used to
	 * communication with the switch
	 */
	const struct mv88e6xxx_bus_ops *smi_ops;
	struct mii_bus *bus;
	int sw_addr;

	/* Handles automatic disabling and re-enabling of the PHY
	 * polling unit.
	 */
	const struct mv88e6xxx_bus_ops *phy_ops;
	struct mutex		ppu_mutex;
	int			ppu_disabled;
	struct work_struct	ppu_work;
	struct timer_list	ppu_timer;

	/* This mutex serialises access to the statistics unit.
	 * Hold this mutex over snapshot + dump sequences.
	 */
	struct mutex	stats_mutex;

	/* A switch may have a GPIO line tied to its reset pin. Parse
	 * this from the device tree, and use it before performing
	 * switch soft reset.
	 */
	struct gpio_desc *reset;

	/* set to size of eeprom if supported by the switch */
	u32 eeprom_len;

	/* List of mdio busses */
	struct list_head mdios;

	/* There can be two interrupt controllers, which are chained
	 * off a GPIO as interrupt source
	 */
	struct mv88e6xxx_irq g1_irq;
	struct mv88e6xxx_irq g2_irq;
	int irq;
	int device_irq;
	int watchdog_irq;

	int atu_prob_irq;
	int vtu_prob_irq;
	struct kthread_worker *kworker;
	struct kthread_delayed_work irq_poll_work;

	/* GPIO resources */
	u8 gpio_data[2];

	/* This cyclecounter abstracts the switch PTP time.
	 * reg_lock must be held for any operation that read()s.
	 */
	struct cyclecounter	tstamp_cc;
	struct timecounter	tstamp_tc;
	struct delayed_work	overflow_work;

	struct ptp_clock	*ptp_clock;
	struct ptp_clock_info	ptp_clock_info;
	struct delayed_work	tai_event_work;
	struct ptp_pin_desc	pin_config[MV88E6XXX_MAX_GPIO];
	u16 trig_config;
	u16 evcap_config;
	u16 enable_count;

	/* Per-port timestamping resources. */
	struct mv88e6xxx_port_hwtstamp port_hwtstamp[DSA_MAX_PORTS];

	/* Array of port structures. */
	struct mv88e6xxx_port ports[DSA_MAX_PORTS];
};

struct mv88e6xxx_bus_ops {
	int (*read)(struct mv88e6xxx_chip *chip, int addr, int reg, u16 *val);
	int (*write)(struct mv88e6xxx_chip *chip, int addr, int reg, u16 val);
};

struct mv88e6xxx_mdio_bus {
	struct mii_bus *bus;
	struct mv88e6xxx_chip *chip;
	struct list_head list;
	bool external;
};

struct mv88e6xxx_ops {
	/* Switch Setup Errata, called early in the switch setup to
	 * allow any errata actions to be performed
	 */
	int (*setup_errata)(struct mv88e6xxx_chip *chip);

	int (*ieee_pri_map)(struct mv88e6xxx_chip *chip);
	int (*ip_pri_map)(struct mv88e6xxx_chip *chip);

	/* Ingress Rate Limit unit (IRL) operations */
	int (*irl_init_all)(struct mv88e6xxx_chip *chip, int port);

	int (*get_eeprom)(struct mv88e6xxx_chip *chip,
			  struct ethtool_eeprom *eeprom, u8 *data);
	int (*set_eeprom)(struct mv88e6xxx_chip *chip,
			  struct ethtool_eeprom *eeprom, u8 *data);

	int (*set_switch_mac)(struct mv88e6xxx_chip *chip, u8 *addr);

	int (*phy_read)(struct mv88e6xxx_chip *chip,
			struct mii_bus *bus,
			int addr, int reg, u16 *val);
	int (*phy_write)(struct mv88e6xxx_chip *chip,
			 struct mii_bus *bus,
			 int addr, int reg, u16 val);

	/* Priority Override Table operations */
	int (*pot_clear)(struct mv88e6xxx_chip *chip);

	/* PHY Polling Unit (PPU) operations */
	int (*ppu_enable)(struct mv88e6xxx_chip *chip);
	int (*ppu_disable)(struct mv88e6xxx_chip *chip);

	/* Switch Software Reset */
	int (*reset)(struct mv88e6xxx_chip *chip);

	/* RGMII Receive/Transmit Timing Control
	 * Add delay on PHY_INTERFACE_MODE_RGMII_*ID, no delay otherwise.
	 */
	int (*port_set_rgmii_delay)(struct mv88e6xxx_chip *chip, int port,
				    phy_interface_t mode);

#define LINK_FORCED_DOWN	0
#define LINK_FORCED_UP		1
#define LINK_UNFORCED		-2

	/* Port's MAC link state
	 * Use LINK_FORCED_UP or LINK_FORCED_DOWN to force link up or down,
	 * or LINK_UNFORCED for normal link detection.
	 */
	int (*port_set_link)(struct mv88e6xxx_chip *chip, int port, int link);

#define DUPLEX_UNFORCED		-2

	/* Port's MAC duplex mode
	 *
	 * Use DUPLEX_HALF or DUPLEX_FULL to force half or full duplex,
	 * or DUPLEX_UNFORCED for normal duplex detection.
	 */
	int (*port_set_duplex)(struct mv88e6xxx_chip *chip, int port, int dup);

#define PAUSE_ON		1
#define PAUSE_OFF		0

	/* Enable/disable sending Pause */
	int (*port_set_pause)(struct mv88e6xxx_chip *chip, int port,
			      int pause);

#define SPEED_MAX		INT_MAX
#define SPEED_UNFORCED		-2

	/* Port's MAC speed (in Mbps)
	 *
	 * Depending on the chip, 10, 100, 200, 1000, 2500, 10000 are valid.
	 * Use SPEED_UNFORCED for normal detection, SPEED_MAX for max value.
	 */
	int (*port_set_speed)(struct mv88e6xxx_chip *chip, int port, int speed);

	int (*port_tag_remap)(struct mv88e6xxx_chip *chip, int port);

	int (*port_set_frame_mode)(struct mv88e6xxx_chip *chip, int port,
				   enum mv88e6xxx_frame_mode mode);
	int (*port_set_egress_floods)(struct mv88e6xxx_chip *chip, int port,
				      bool unicast, bool multicast);
	int (*port_set_ether_type)(struct mv88e6xxx_chip *chip, int port,
				   u16 etype);
	int (*port_set_jumbo_size)(struct mv88e6xxx_chip *chip, int port,
				   size_t size);

	int (*port_egress_rate_limiting)(struct mv88e6xxx_chip *chip, int port);
	int (*port_pause_limit)(struct mv88e6xxx_chip *chip, int port, u8 in,
				u8 out);
	int (*port_disable_learn_limit)(struct mv88e6xxx_chip *chip, int port);
	int (*port_disable_pri_override)(struct mv88e6xxx_chip *chip, int port);

	/* CMODE control what PHY mode the MAC will use, eg. SGMII, RGMII, etc.
	 * Some chips allow this to be configured on specific ports.
	 */
	int (*port_set_cmode)(struct mv88e6xxx_chip *chip, int port,
			      phy_interface_t mode);
	int (*port_get_cmode)(struct mv88e6xxx_chip *chip, int port, u8 *cmode);

	/* Some devices have a per port register indicating what is
	 * the upstream port this port should forward to.
	 */
	int (*port_set_upstream_port)(struct mv88e6xxx_chip *chip, int port,
				      int upstream_port);
	/* Return the port link state, as required by phylink */
	int (*port_link_state)(struct mv88e6xxx_chip *chip, int port,
			       struct phylink_link_state *state);

	/* Snapshot the statistics for a port. The statistics can then
	 * be read back a leisure but still with a consistent view.
	 */
	int (*stats_snapshot)(struct mv88e6xxx_chip *chip, int port);

	/* Set the histogram mode for statistics, when the control registers
	 * are separated out of the STATS_OP register.
	 */
	int (*stats_set_histogram)(struct mv88e6xxx_chip *chip);

	/* Return the number of strings describing statistics */
	int (*stats_get_sset_count)(struct mv88e6xxx_chip *chip);
	int (*stats_get_strings)(struct mv88e6xxx_chip *chip,  uint8_t *data);
	int (*stats_get_stats)(struct mv88e6xxx_chip *chip,  int port,
			       uint64_t *data);
	int (*set_cpu_port)(struct mv88e6xxx_chip *chip, int port);
	int (*set_egress_port)(struct mv88e6xxx_chip *chip, int port);

#define MV88E6XXX_CASCADE_PORT_NONE		0xe
#define MV88E6XXX_CASCADE_PORT_MULTIPLE		0xf

	int (*set_cascade_port)(struct mv88e6xxx_chip *chip, int port);

	const struct mv88e6xxx_irq_ops *watchdog_ops;

	int (*mgmt_rsvd2cpu)(struct mv88e6xxx_chip *chip);

	/* Power on/off a SERDES interface */
	int (*serdes_power)(struct mv88e6xxx_chip *chip, int port, bool on);

	/* SERDES interrupt handling */
	int (*serdes_irq_setup)(struct mv88e6xxx_chip *chip, int port);
	void (*serdes_irq_free)(struct mv88e6xxx_chip *chip, int port);

	/* Statistics from the SERDES interface */
	int (*serdes_get_sset_count)(struct mv88e6xxx_chip *chip, int port);
	int (*serdes_get_strings)(struct mv88e6xxx_chip *chip,  int port,
				  uint8_t *data);
	int (*serdes_get_stats)(struct mv88e6xxx_chip *chip,  int port,
				uint64_t *data);

	/* VLAN Translation Unit operations */
	int (*vtu_getnext)(struct mv88e6xxx_chip *chip,
			   struct mv88e6xxx_vtu_entry *entry);
	int (*vtu_loadpurge)(struct mv88e6xxx_chip *chip,
			     struct mv88e6xxx_vtu_entry *entry);

	/* GPIO operations */
	const struct mv88e6xxx_gpio_ops *gpio_ops;

	/* Interface to the AVB/PTP registers */
	const struct mv88e6xxx_avb_ops *avb_ops;

	/* Remote Management Unit operations */
	int (*rmu_disable)(struct mv88e6xxx_chip *chip);

	/* Precision Time Protocol operations */
	const struct mv88e6xxx_ptp_ops *ptp_ops;

	/* Phylink */
	void (*phylink_validate)(struct mv88e6xxx_chip *chip, int port,
				 unsigned long *mask,
				 struct phylink_link_state *state);
};

struct mv88e6xxx_irq_ops {
	/* Action to be performed when the interrupt happens */
	int (*irq_action)(struct mv88e6xxx_chip *chip, int irq);
	/* Setup the hardware to generate the interrupt */
	int (*irq_setup)(struct mv88e6xxx_chip *chip);
	/* Reset the hardware to stop generating the interrupt */
	void (*irq_free)(struct mv88e6xxx_chip *chip);
};

struct mv88e6xxx_gpio_ops {
	/* Get/set data on GPIO pin */
	int (*get_data)(struct mv88e6xxx_chip *chip, unsigned int pin);
	int (*set_data)(struct mv88e6xxx_chip *chip, unsigned int pin,
			int value);

	/* get/set GPIO direction */
	int (*get_dir)(struct mv88e6xxx_chip *chip, unsigned int pin);
	int (*set_dir)(struct mv88e6xxx_chip *chip, unsigned int pin,
		       bool input);

	/* get/set GPIO pin control */
	int (*get_pctl)(struct mv88e6xxx_chip *chip, unsigned int pin,
			int *func);
	int (*set_pctl)(struct mv88e6xxx_chip *chip, unsigned int pin,
			int func);
};

struct mv88e6xxx_avb_ops {
	/* Access port-scoped Precision Time Protocol registers */
	int (*port_ptp_read)(struct mv88e6xxx_chip *chip, int port, int addr,
			     u16 *data, int len);
	int (*port_ptp_write)(struct mv88e6xxx_chip *chip, int port, int addr,
			      u16 data);

	/* Access global Precision Time Protocol registers */
	int (*ptp_read)(struct mv88e6xxx_chip *chip, int addr, u16 *data,
			int len);
	int (*ptp_write)(struct mv88e6xxx_chip *chip, int addr, u16 data);

	/* Access global Time Application Interface registers */
	int (*tai_read)(struct mv88e6xxx_chip *chip, int addr, u16 *data,
			int len);
	int (*tai_write)(struct mv88e6xxx_chip *chip, int addr, u16 data);
};

struct mv88e6xxx_ptp_ops {
	u64 (*clock_read)(const struct cyclecounter *cc);
	int (*ptp_enable)(struct ptp_clock_info *ptp,
			  struct ptp_clock_request *rq, int on);
	int (*ptp_verify)(struct ptp_clock_info *ptp, unsigned int pin,
			  enum ptp_pin_function func, unsigned int chan);
	void (*event_work)(struct work_struct *ugly);
	int (*port_enable)(struct mv88e6xxx_chip *chip, int port);
	int (*port_disable)(struct mv88e6xxx_chip *chip, int port);
	int (*global_enable)(struct mv88e6xxx_chip *chip);
	int (*global_disable)(struct mv88e6xxx_chip *chip);
	int n_ext_ts;
	int arr0_sts_reg;
	int arr1_sts_reg;
	int dep_sts_reg;
	u32 rx_filters;
};

#define STATS_TYPE_PORT		BIT(0)
#define STATS_TYPE_BANK0	BIT(1)
#define STATS_TYPE_BANK1	BIT(2)

struct mv88e6xxx_hw_stat {
	char string[ETH_GSTRING_LEN];
	size_t size;
	int reg;
	int type;
};

static inline bool mv88e6xxx_has_pvt(struct mv88e6xxx_chip *chip)
{
	return chip->info->pvt;
}

static inline unsigned int mv88e6xxx_num_databases(struct mv88e6xxx_chip *chip)
{
	return chip->info->num_databases;
}

static inline unsigned int mv88e6xxx_num_ports(struct mv88e6xxx_chip *chip)
{
	return chip->info->num_ports;
}

static inline u16 mv88e6xxx_port_mask(struct mv88e6xxx_chip *chip)
{
	return GENMASK(mv88e6xxx_num_ports(chip) - 1, 0);
}

static inline unsigned int mv88e6xxx_num_gpio(struct mv88e6xxx_chip *chip)
{
	return chip->info->num_gpio;
}

int mv88e6xxx_read(struct mv88e6xxx_chip *chip, int addr, int reg, u16 *val);
int mv88e6xxx_write(struct mv88e6xxx_chip *chip, int addr, int reg, u16 val);
int mv88e6xxx_update(struct mv88e6xxx_chip *chip, int addr, int reg,
		     u16 update);
int mv88e6xxx_wait(struct mv88e6xxx_chip *chip, int addr, int reg, u16 mask);
struct mii_bus *mv88e6xxx_default_mdio_bus(struct mv88e6xxx_chip *chip);

#endif /* _MV88E6XXX_CHIP_H */
