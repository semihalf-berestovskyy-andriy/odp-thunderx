/* Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

/**
 * @file
 *
 * ODP IPSEC API
 */

#ifndef ODP_API_IPSEC_H_
#define ODP_API_IPSEC_H_
#include <odp/visibility_begin.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <odp/api/crypto.h>

/** @defgroup odp_ipsec ODP IPSEC
 *  Operations of IPSEC API.
 *  @{
 */

/**
 * @typedef odp_ipsec_sa_t
 * IPSEC Security Association (SA)
 */

 /**
 * @def ODP_IPSEC_SA_INVALID
 * Invalid IPSEC SA
 */

/**
 * IPSEC operation mode
 */
typedef enum odp_ipsec_op_mode_t {
	/** Synchronous IPSEC operation
	  *
	  * Application uses synchronous IPSEC operations,
	  * which output all results on function return.
	  */
	ODP_IPSEC_OP_MODE_SYNC = 0,

	/** Asynchronous IPSEC operation
	  *
	  * Application uses asynchronous IPSEC operations,
	  * which return results via events.
	  */
	ODP_IPSEC_OP_MODE_ASYNC

} odp_ipsec_op_mode_t;

/**
 * IPSEC capability
 */
typedef struct odp_ipsec_capability_t {
	/** Maximum number of IPSEC SAs */
	uint32_t max_num_sa;

	/** Synchronous IPSEC operation mode (ODP_IPSEC_OP_MODE_SYNC) support
	 *
	 *  0: Synchronous mode is not supported
	 *  1: Synchronous mode is supported
	 *  2: Synchronous mode is supported and preferred
	 */
	uint8_t op_mode_sync;

	/** Asynchronous IPSEC operation mode (ODP_IPSEC_OP_MODE_ASYNC) support
	 *
	 *  0: Asynchronous mode is not supported
	 *  1: Asynchronous mode is supported
	 *  2: Asynchronous mode is supported and preferred
	 */
	uint8_t op_mode_async;

	/** Soft expiry limit in seconds support
	 *
	 *  0: Limit is not supported
	 *  1: Limit is supported
	 */
	uint8_t soft_limit_sec;

	/** Hard expiry limit in seconds support
	 *
	 *  0: Limit is not supported
	 *  1: Limit is supported
	 */
	uint8_t hard_limit_sec;

	/** Supported cipher algorithms */
	odp_crypto_cipher_algos_t ciphers;

	/** Supported authentication algorithms */
	odp_crypto_auth_algos_t   auths;

} odp_ipsec_capability_t;

/**
 * IPSEC configuration options
 */
typedef struct odp_ipsec_config_t {
	/** IPSEC operation mode. Application selects which mode (sync or async)
	 *  will be used for IPSEC operations.
	 *
	 *  @see odp_ipsec_in(), odp_ipsec_in_enq()
	 */
	odp_ipsec_op_mode_t op_mode;

} odp_ipsec_config_t;

/**
 * IPSEC SA direction
 */
typedef enum odp_ipsec_dir_t {
	/** Inbound IPSEC SA */
	ODP_IPSEC_DIR_INBOUND = 0,

	/** Outbound IPSEC SA */
	ODP_IPSEC_DIR_OUTBOUND

} odp_ipsec_dir_t;

/**
 * IPSEC protocol mode
 */
typedef enum odp_ipsec_mode_t {
	/** IPSEC tunnel mode */
	ODP_IPSEC_MODE_TUNNEL = 0,

	/** IPSEC transport mode */
	ODP_IPSEC_MODE_TRANSPORT

} odp_ipsec_mode_t;

/**
 * IPSEC protocol
 */
typedef enum odp_ipsec_protocol_t {
	/** ESP protocol */
	ODP_IPSEC_ESP = 0,

	/** AH protocol */
	ODP_IPSEC_AH

} odp_ipsec_protocol_t;

/**
 * IPSEC tunnel type
 */
typedef enum odp_ipsec_tunnel_type_t {
	/** Outer header is IPv4 */
	ODP_IPSEC_TUNNEL_IPV4 = 0,

	/** Outer header is IPv6 */
	ODP_IPSEC_TUNNEL_IPV6

} odp_ipsec_tunnel_type_t;

/**
 * IPSEC crypto parameters
 */
typedef struct odp_ipsec_crypto_param_t {
	/** Cipher algorithm */
	odp_cipher_alg_t cipher_alg;

	/** Cipher key */
	odp_crypto_key_t cipher_key;

	/** Authentication algorithm */
	odp_auth_alg_t auth_alg;

	/** Authentication key */
	odp_crypto_key_t auth_key;

} odp_ipsec_crypto_param_t;

/**
 * IPSEC tunnel parameters
 *
 * These parameters are used to build outbound tunnel headers. All values are
 * passed in CPU native byte / bit order if not specified otherwise.
 * IP addresses must be in NETWORK byte order as those are passed in with
 * pointers and copied byte-by-byte from memory to the packet.
 */
typedef struct odp_ipsec_tunnel_param_t {
	/** Tunnel type: IPv4 or IPv6 */
	odp_ipsec_tunnel_type_t type;

	union {
		/** IPv4 header parameters */
		struct {
			/** IPv4 source address (NETWORK ENDIAN) */
			void *src_addr;

			/** IPv4 destination address (NETWORK ENDIAN) */
			void *dst_addr;

			/** IPv4 Differentiated Services Code Point */
			uint8_t dscp;

			/** IPv4 Don't Fragment bit */
			uint8_t df;

			/** IPv4 Time To Live */
			uint8_t ttl;
		} ipv4;

		/** IPv6 header parameters */
		struct {
			/** IPv6 source address (NETWORK ENDIAN) */
			void *src_addr;

			/** IPv6 destination address (NETWORK ENDIAN) */
			void *dst_addr;

			/** IPv6 Differentiated Services Code Point */
			uint8_t dscp;

			/** IPv6 flow label */
			uint32_t flabel;

			/** IPv6 hop limit */
			uint8_t hlimit;
		} ipv6;
	};
} odp_ipsec_tunnel_param_t;

/**
 * IPSEC SA option flags
 */
typedef struct odp_ipsec_sa_opt_t {
	/** Extended Sequence Numbers (ESN)
	  *
	  * * 1: Use extended (64 bit) sequence numbers
	  * * 0: Use normal sequence numbers
	  */
	uint32_t esn : 1;

	/** UDP encapsulation
	  *
	  * * 1: Do UDP encapsulation/decapsulation so that IPSEC packets can
	  *      traverse through NAT boxes.
	  * * 0: No UDP encapsulation
	  */
	uint32_t udp_encap : 1;

	/** Copy DSCP bits
	  *
	  * * 1: Copy IPv4 or IPv6 DSCP bits from inner IP header to
	  *      the outer IP header in encapsulation, and vice versa in
	  *      decapsulation.
	  * * 0: Use values from odp_ipsec_tunnel_param_t in encapsulation and
	  *      do not change DSCP field in decapsulation.
	  */
	uint32_t copy_dscp : 1;

	/** Copy IPv6 Flow Label
	  *
	  * * 1: Copy IPv6 flow label from inner IPv6 header to the
	  *      outer IPv6 header.
	  * * 0: Use value from odp_ipsec_tunnel_param_t
	  */
	uint32_t copy_flabel : 1;

	/** Copy IPv4 Don't Fragment bit
	  *
	  * * 1: Copy the DF bit from the inner IPv4 header to the outer
	  *      IPv4 header.
	  * * 0: Use value from odp_ipsec_tunnel_param_t
	  */
	uint32_t copy_df : 1;

	/** Decrement inner packet Time To Live (TTL) field
	  *
	  * * 1: In tunnel mode, decrement inner packet IPv4 TTL or
	  *      IPv6 Hop Limit after tunnel decapsulation, or before tunnel
	  *      encapsulation.
	  * * 0: Inner packet is not modified.
	  */
	uint32_t dec_ttl : 1;

} odp_ipsec_sa_opt_t;

/**
 * IPSEC SA lifetime limits
 *
 * These limits are used for setting up SA lifetime. IPSEC operations check
 * against the limits and output a status code (e.g. soft_exp_bytes) when
 * a limit is crossed. Any number of limits may be used simultaneously.
 * Use zero when there is no limit.
 */
typedef struct odp_ipsec_lifetime_t {
	/** Soft expiry limits for the session */
	struct {
		/** Limit in seconds from the SA creation */
		uint64_t sec;

		/** Limit in bytes */
		uint64_t bytes;

		/** Limit in packet */
		uint64_t packets;
	} soft_limit;

	/** Hard expiry limits for the session */
	struct {
		/** Limit in seconds from the SA creation */
		uint64_t sec;

		/** Limit in bytes */
		uint64_t bytes;

		/** Limit in packet */
		uint64_t packets;
	} hard_limit;
} odp_ipsec_lifetime_t;

/**
 * Fragmentation mode
 *
 * These options control outbound IP packet fragmentation offload. When offload
 * is enabled, IPSEC operation will determine if fragmentation is needed and
 * does it according to the mode.
 */
typedef enum odp_ipsec_frag_mode_t {
	/** Do not fragment IP packets */
	ODP_IPSEC_FRAG_DISABLED = 0,

	/** Fragment IP packet before IPSEC operation */
	ODP_IPSEC_FRAG_BEFORE,

	/** Fragment IP packet after IPSEC operation */
	ODP_IPSEC_FRAG_AFTER,

	/** Only check if IP fragmentation is needed,
	  * do not fragment packets. */
	ODP_IPSEC_FRAG_CHECK
} odp_ipsec_frag_mode_t;

/**
 * Packet lookup mode
 */
typedef enum odp_ipsec_lookup_mode_t {
	/** Inbound SA lookup is disabled. */
	ODP_IPSEC_LOOKUP_DISABLED = 0,

	/** Inbound SA lookup is enabled. Used SPI values must be unique. */
	ODP_IPSEC_LOOKUP_IN_UNIQUE_SA

} odp_ipsec_lookup_mode_t;

/**
 * IPSEC Security Association (SA) parameters
 */
typedef struct odp_ipsec_sa_param_t {
	/** IPSEC SA direction: inbound or outbound */
	odp_ipsec_dir_t dir;

	/** IPSEC protocol: ESP or AH */
	odp_ipsec_protocol_t proto;

	/** IPSEC protocol mode: transport or tunnel */
	odp_ipsec_mode_t mode;

	/** Parameters for crypto and authentication algorithms */
	odp_ipsec_crypto_param_t crypto;

	/** Parameters for tunnel mode */
	odp_ipsec_tunnel_param_t tunnel;

	/** Fragmentation mode */
	odp_ipsec_frag_mode_t frag_mode;

	/** Various SA option flags */
	odp_ipsec_sa_opt_t opt;

	/** SA lifetime parameters */
	odp_ipsec_lifetime_t lifetime;

	/** SA lookup mode */
	odp_ipsec_lookup_mode_t lookup_mode;

	/** Minimum anti-replay window size. Use 0 to disable anti-replay
	  * service. */
	uint32_t antireplay_ws;

	/** Initial sequence number */
	uint64_t seq;

	/** SPI value */
	uint32_t spi;

	/** MTU for outbound IP fragmentation offload
	 *
	 *  This is the maximum length of IP packets that outbound IPSEC
	 *  operations may produce. The value may be updated later with
	 *  odp_ipsec_mtu_update().
	 */
	uint32_t mtu;

	/** Destination queue for IPSEC events
	 *
	 *  Operations in asynchronous mode enqueue resulting events into
	 *  this queue.
	 */
	odp_queue_t dest_queue;

	/** User defined SA context pointer
	 *
	 *  User defined context pointer associated with the SA.
	 *  The implementation may prefetch the context data. Default value
	 *  of the pointer is NULL.
	 */
	void *context;

	/** Context data length
	 *
	 *  User defined context data length in bytes for prefetching.
	 *  The implementation may use this value as a hint for the number of
	 *  context data bytes to prefetch. Default value is zero (no hint).
	 */
	uint32_t context_len;

} odp_ipsec_sa_param_t;

/**
 * Query IPSEC capabilities
 *
 * Outputs IPSEC capabilities on success.
 *
 * @param[out] capa   Pointer to capability structure for output
 *
 * @retval 0 on success
 * @retval <0 on failure
 */
int odp_ipsec_capability(odp_ipsec_capability_t *capa);

/**
 * Initialize IPSEC configuration options
 *
 * Initialize an odp_ipsec_config_t to its default values.
 *
 * @param[out] config  Pointer to IPSEC configuration structure
 */
void odp_ipsec_config_init(odp_ipsec_config_t *config);

/**
 * Global IPSEC configuration
 *
 * Initialize and configure IPSEC offload with global configuration options.
 * This must be called before any SAs are created. Use odp_ipsec_capability()
 * to examine which features and modes are supported.
 *
 * @param config   Pointer to IPSEC configuration structure
 *
 * @retval 0 on success
 * @retval <0 on failure
 *
 * @see odp_ipsec_capability(), odp_ipsec_config_init()
 */
int odp_ipsec_config(const odp_ipsec_config_t *config);

/**
 * Initialize IPSEC SA parameters
 *
 * Initialize an odp_ipsec_sa_param_t to its default values for all fields.
 *
 * @param param   Pointer to the parameter structure
 */
void odp_ipsec_sa_param_init(odp_ipsec_sa_param_t *param);

/**
 * Create IPSEC SA
 *
 * Create a new IPSEC SA according to the parameters.
 *
 * @param param   IPSEC SA parameters
 *
 * @return IPSEC SA handle
 * @retval ODP_IPSEC_SA_INVALID on failure
 *
 * @see odp_ipsec_sa_param_init()
 */
odp_ipsec_sa_t odp_ipsec_sa_create(odp_ipsec_sa_param_t *param);

/**
 * Destroy IPSEC SA
 *
 * Destroy an unused IPSEC SA. Result is undefined if the SA is being used
 * (i.e. asynchronous operation is in progress).
 *
 * @param sa      IPSEC SA to be destroyed
 *
 * @retval 0      On success
 * @retval <0     On failure
 *
 * @see odp_ipsec_sa_create()
 */
int odp_ipsec_sa_destroy(odp_ipsec_sa_t sa);

/**
 * Printable format of odp_ipsec_sa_t
 *
 * @param sa      IPSEC SA handle
 *
 * @return uint64_t value that can be used to print/display this handle
 */
uint64_t odp_ipsec_sa_to_u64(odp_ipsec_sa_t sa);

/**
 * IPSEC operation level options
 *
 * These may be used to override some SA level options
 */
typedef struct odp_ipsec_op_opt_t {
	/** Fragmentation mode */
	odp_ipsec_frag_mode_t mode;

} odp_ipsec_op_opt_t;

/** IPSEC operation status has no errors */
#define ODP_IPSEC_OK 0

/** IPSEC operation status */
typedef union odp_ipsec_status_t {
	/** Error flags */
	struct {
		/** Protocol error. Not a valid ESP or AH packet. */
		uint32_t proto            : 1;

		/** SA lookup failed */
		uint32_t sa_lookup        : 1;

		/** Authentication failed */
		uint32_t auth             : 1;

		/** Anti-replay check failed */
		uint32_t antireplay       : 1;

		/** Other algorithm error */
		uint32_t alg              : 1;

		/** Packet does not fit into the given MTU size */
		uint32_t mtu              : 1;

		/** Soft lifetime expired: seconds */
		uint32_t soft_exp_sec     : 1;

		/** Soft lifetime expired: bytes */
		uint32_t soft_exp_bytes   : 1;

		/** Soft lifetime expired: packets */
		uint32_t soft_exp_packets : 1;

		/** Hard lifetime expired: seconds */
		uint32_t hard_exp_sec     : 1;

		/** Hard lifetime expired: bytes */
		uint32_t hard_exp_bytes   : 1;

		/** Hard lifetime expired: packets */
		uint32_t hard_exp_packets : 1;
	} error;

	/** All bits of the bit field structure
	  *
	  * This field can be used to set, clear or compare multiple flags.
	  * For example, 'status.all != ODP_IPSEC_OK' checks if there are any
	  * errors.
	  */
	uint32_t all;

} odp_ipsec_status_t;

/**
 * IPSEC operation input parameters
 */
typedef struct odp_ipsec_op_param_t {
	/** Number of packets to be processed */
	int num_pkt;

	/** Number of SAs
	 *
	 *  Valid values are:
	 *  * 0:       No SAs (default)
	 *  * 1:       Single SA for all packets
	 *  * num_pkt: SA per packet
	 */
	int num_sa;

	/** Number of operation options
	 *
	 *  Valid values are:
	 *  * 0:       No options (default)
	 *  * 1:       Single option for all packets
	 *  * num_pkt: An option per packet
	 */
	int num_opt;

	/** Pointer to an array of packets
	 *
	 *  Each packet must have a valid value for these meta-data:
	 *  * L3 offset: Offset to the first byte of the (outmost) IP header
	 *  * L4 offset: For inbound direction, when udp_encap is enabled -
	 *               offset to the first byte of the encapsulating UDP
	 *               header
	 *
	 *  @see odp_packet_l3_offset(), odp_packet_l4_offset()
	 */
	odp_packet_t *pkt;

	/** Pointer to an array of IPSEC SAs
	 *
	 *  May be NULL when num_sa is zero.
	 */
	odp_ipsec_sa_t *sa;

	/** Pointer to an array of operation options
	 *
	 *  May be NULL when num_opt is zero.
	 */
	odp_ipsec_op_opt_t *opt;

} odp_ipsec_op_param_t;

/**
 * IPSEC operation result for a packet
 */
typedef struct odp_ipsec_packet_result_t {
	/** IPSEC operation status */
	odp_ipsec_status_t status;

	/** Number of output packets created from the corresponding input packet
	 *
	 *  Without fragmentation offload this is always one. However, if the
	 *  input packet was fragmented during the operation this is larger than
	 *  one for the first fragment and zero for the rest of the fragments
	 *  (following the first one in the 'pkt' array).
	 */
	int num_out;

	/** IPSEC SA that was used to create the packet
	 *
	 *  Operation updates this SA handle value, when SA look up is performed
	 *  as part of the operation and the look up is successful. Operation
	 *  status code indicates if the look up failed. Otherwise, the SA
	 *  provided by the application is copied here.
	 */
	odp_ipsec_sa_t sa;

} odp_ipsec_packet_result_t;

/**
 * IPSEC operation results
 */
typedef struct odp_ipsec_op_result_t {
	/** Number of packets
	 *
	 *  Application sets this to the maximum number of packets the operation
	 *  may output (number of elements in 'pkt' and 'res' arrays).
	 *  The operation updates it with the actual number of packets
	 *  outputted.
	 */
	int num_pkt;

	/** Pointer to an array of packets
	 *
	 *  Operation outputs packets into this array. The array must have
	 *  at least 'num_pkt' elements.
	 *
	 *  Each successfully transformed packet has a valid value for these
	 *  meta-data:
	 *  * L3 offset: Offset to the first byte of the (outmost) IP header
	 *  * L4 offset: Offset to the first byte of the valid and known L4
	 *               header (immediately following the IP header).
	 *  * Various flags about L3 and L4 layers:
	 *               has_l3, has_l4, has_ipv4, has_ipv6, has_ipfrag,
	 *               has_ipsec, has_udp, has_tcp, etc depending on
	 *               the resulted packet format
	 *
	 * @see odp_packet_l3_offset(), odp_packet_l4_offset(),
	 *      odp_packet_has_ipv4(), odp_packet_has_ipv6(),
	 *      odp_packet_has_ipfrag(), odp_packet_has_ipsec()
	 *
	 * @note The amount and content of packet data before the IP header is
	 *       implementation specific.
	 */
	odp_packet_t *pkt;

	/** Pointer to an array of per packet operation results
	 *
	 *  Operation outputs results for each outputted packet into this array.
	 *  The array must have at least 'num_pkt' elements. The results include
	 *  operation status and packet form information for each outputted
	 *  packet.
	 *
	 *  For example, some packets may not have been transformed due to
	 *  an error, but the original packet is returned with appropriate
	 *  packet result information instead.
	 */
	odp_ipsec_packet_result_t *res;

} odp_ipsec_op_result_t;

/**
 * Inbound synchronous IPSEC operation
 *
 * This operation does inbound IPSEC processing in synchronous mode
 * (ODP_IPSEC_OP_MODE_SYNC). A successful operation returns the number of
 * packets consumed and outputs a new packet handle as well as an operation
 * result for each outputted packet. The operation does not modify packets that
 * it does not consume. It cannot consume all input packets if 'output.num_pkt'
 * is smaller than 'input.num_pkt'.
 *
 * Packet context pointer and user area content are copied from input to output
 * packets. Output packets are allocated from the same pool(s) as input packets.
 *
 * When 'input.num_sa' is zero, this operation performs SA look up for each
 * packet. Otherwise, application must provide the SA(s) as part of operation
 * input parameters (odp_ipsec_op_param_t). The operation outputs used SA(s) as
 * part of per packet operation results (odp_ipsec_packet_result_t), or an error
 * status if a SA was not found.
 *
 * Packets are processed in the input order. Packet order is maintained from
 * input 'pkt' array to output 'pkt' array. Packet order is not guaranteed
 * between calling threads.
 *
 * @param         input   Operation input parameters
 * @param[out]    output  Operation results
 *
 * @return Number of input packets consumed (0 ... input.num_pkt)
 * @retval <0     On failure
 *
 * @see odp_packet_user_ptr(), odp_packet_user_area()
 */
int odp_ipsec_in(const odp_ipsec_op_param_t *input,
		 odp_ipsec_op_result_t *output);

/**
 * Outbound synchronous IPSEC operation
 *
 * This operation does outbound IPSEC processing in synchronous mode
 * (ODP_IPSEC_OP_MODE_SYNC). A successful operation returns the number of
 * packets consumed and outputs a new packet handle as well as an operation
 * result for each outputted packet. The operation does not modify packets that
 * it does not consume. It cannot consume all input packets if 'output.num_pkt'
 * is smaller than 'input.num_pkt'.
 *
 * Packet context pointer and user area content are copied from input to output
 * packets. Output packets are allocated from the same pool(s) as input packets.
 *
 * When outbound IP fragmentation offload is enabled, the number of outputted
 * packets (and corresponding per packet results) may be greater than
 * the number of input packets. In that case, application may examine 'num_out'
 * of each packet result (odp_ipsec_packet_result_t) to find out which
 * fragments are originated from which input packet.
 *
 * Packets are processed in the input order. Packet order is maintained from
 * input 'pkt' array to output 'pkt' array. Packet order is not guaranteed
 * between calling threads.
 *
 * @param         input   Operation input parameters
 * @param[out]    output  Operation results
 *
 * @return Number of input packets consumed (0 ... input.num_pkt)
 * @retval <0     On failure
 *
 * @see odp_packet_user_ptr(), odp_packet_user_area()
 */
int odp_ipsec_out(const odp_ipsec_op_param_t *input,
		  odp_ipsec_op_result_t *output);

/**
 * Inbound asynchronous IPSEC operation
 *
 * This operation does inbound IPSEC processing in asynchronous mode
 * (ODP_IPSEC_OP_MODE_ASYNC). It processes packets otherwise identically to
 * odp_ipsec_in(), but outputs all results through one or more
 * ODP_EVENT_IPSEC_RESULT events with the following ordering considerations.
 *
 * Asynchronous mode maintains (operation input) packet order per SA when
 * application calls the operation within an ordered or atomic scheduler context
 * of the same queue. Packet order is also maintained when application
 * otherwise guarantees (e.g. using locks) that the operation is not called
 * simultaneously from multiple threads for the same SA(s). Resulting
 * events for the same SA are enqueued in order, and packet handles (for the
 * same SA) are stored in order within an event.
 *
 * @param         input   Operation input parameters
 *
 * @return Number of input packets consumed (0 ... input.num_pkt)
 * @retval <0     On failure
 *
 * @see odp_ipsec_in(), odp_ipsec_result()
 */
int odp_ipsec_in_enq(const odp_ipsec_op_param_t *input);

/**
 * Outbound asynchronous IPSEC operation
 *
 * This operation does outbound IPSEC processing in asynchronous mode
 * (ODP_IPSEC_OP_MODE_ASYNC). It processes packets otherwise identically to
 * odp_ipsec_out(), but outputs all results through one or more
 * ODP_EVENT_IPSEC_RESULT events with the following ordering considerations.
 *
 * Asynchronous mode maintains (operation input) packet order per SA when
 * application calls the operation within an ordered or atomic scheduler context
 * of the same queue. Packet order is also maintained when application
 * otherwise guarantees (e.g. using locks) that the operation is not called
 * simultaneously from multiple threads for the same SA(s). Resulting
 * events for the same SA are enqueued in order, and packet handles (for the
 * same SA) are stored in order within an event.
 *
 * @param         input   Operation input parameters
 *
 * @return Number of input packets consumed (0 ... input.num_pkt)
 * @retval <0     On failure
 *
 * @see odp_ipsec_out(), odp_ipsec_result()
 */
int odp_ipsec_out_enq(const odp_ipsec_op_param_t *input);

/**
 * Get IPSEC results from an ODP_EVENT_IPSEC_RESULT event
 *
 * Copies IPSEC operation results from an event. The event must be of
 * type ODP_EVENT_IPSEC_RESULT. It must be freed before the application passes
 * any resulting packet handles to other ODP calls.
 *
 * @param[out]    result  Pointer to operation result for output. Maybe NULL, if
 *                        application is interested only on the number of
 *                        packets.
 * @param         event   An ODP_EVENT_IPSEC_RESULT event
 *
 * @return Number of packets in the event. If this is larger than
 *         'result.num_pkt', all packets did not fit into result struct and
 *         application must call the function again with a larger result struct.
 * @retval <0     On failure
 *
 * @see odp_ipsec_in_enq(), odp_ipsec_out_enq()
 */
int odp_ipsec_result(odp_ipsec_op_result_t *result, odp_event_t event);

/**
 * Update MTU for outbound IP fragmentation
 *
 * When IP fragmentation offload is enabled, the SA is created with an MTU.
 * This call may be used to update MTU at any time. MTU updates are not
 * expected to happen very frequently.
 *
 * @param sa      IPSEC SA to be updated
 * @param mtu     The new MTU value
 *
 * @retval 0      On success
 * @retval <0     On failure
 */
int odp_ipsec_mtu_update(odp_ipsec_sa_t sa, uint32_t mtu);

/**
 * Get user defined SA context pointer
 *
 * @param sa      IPSEC SA handle
 *
 * @return User defined SA context pointer value
 * @retval NULL   On failure
 */
void *odp_ipsec_sa_context(odp_ipsec_sa_t sa);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#include <odp/visibility_end.h>
#endif
