/* Copyright (c) 2017, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#include <odp/api/ipsec.h>
#include <odp/api/crypto.h>
#include <odp_debug_internal.h>

#include <string.h>

#define OP_MODE_ASYNC_AVAILABLE		0

/* to be changed when implementing SAs */
#define MAX_NUM_SA			0

int odp_ipsec_capability(odp_ipsec_capability_t *capa)
{
	odp_crypto_capability_t crypto_cap;

	if (odp_crypto_capability(&crypto_cap)) {
		ODP_ERR("Error querying crypto capabilities\n");
		return -1;
	}

	capa->max_num_sa 	= (MAX_NUM_SA < crypto_cap.max_sessions) 
					? MAX_NUM_SA : crypto_cap.max_sessions;
	capa->op_mode_sync 	= 1;
	capa->op_mode_async 	= OP_MODE_ASYNC_AVAILABLE;
	capa->soft_limit_sec 	= 0;
	capa->hard_limit_sec 	= 0;
	capa->ciphers		= crypto_cap.ciphers;
	capa->auths		= crypto_cap.auths;

	return 0;
}

int odp_ipsec_cipher_capability(odp_cipher_alg_t cipher,
				odp_crypto_cipher_capability_t capa[], int num)
{
	return odp_crypto_cipher_capability(cipher, capa, num);
}

int odp_ipsec_auth_capability(odp_auth_alg_t auth,
			      odp_crypto_auth_capability_t capa[], int num)
{
	return odp_crypto_auth_capability(auth, capa, num);
}

void odp_ipsec_config_init(odp_ipsec_config_t *config ODP_UNUSED)
{
	config->op_mode = ODP_IPSEC_OP_MODE_SYNC;
}

int odp_ipsec_config(const odp_ipsec_config_t *config) {
#if !(OP_MODE_ASYNC_AVAILABLE)
	if (ODP_IPSEC_OP_MODE_ASYNC == 
			config->op_mode) {
		ODP_ERR("Asynchronous mode not available\n");
		return -1;
	}
#elif
	if (ODP_IPSEC_OP_MODE_ASYNC ==
			config->op_mode) {
		/* some potential setup? */
	}
#endif
	return 0;
}

void odp_ipsec_sa_param_init(odp_ipsec_sa_param_t *param ODP_UNUSED)
{
	ODP_UNIMPLEMENTED();
}

odp_ipsec_sa_t odp_ipsec_sa_create(odp_ipsec_sa_param_t *param ODP_UNUSED)
{
	ODP_UNIMPLEMENTED();
	odp_ipsec_sa_t sa = 0;
	return sa;
}

int odp_ipsec_sa_destroy(odp_ipsec_sa_t sa ODP_UNUSED)
{
	ODP_UNIMPLEMENTED();
	return 0;
}

uint64_t odp_ipsec_sa_to_u64(odp_ipsec_sa_t sa ODP_UNUSED)
{
	ODP_UNIMPLEMENTED();
	return 0;
}

int odp_ipsec_in(const odp_ipsec_op_param_t *input ODP_UNUSED,
		 odp_ipsec_op_result_t *output ODP_UNUSED)
{
	ODP_UNIMPLEMENTED();
	return 0;
}

int odp_ipsec_out(const odp_ipsec_op_param_t *input ODP_UNUSED,
		  odp_ipsec_op_result_t *output ODP_UNUSED)
{
	ODP_UNIMPLEMENTED();
	return 0;
}


int odp_ipsec_in_enq(const odp_ipsec_op_param_t *input ODP_UNUSED)
{
	ODP_UNIMPLEMENTED();
#if !(OP_MODE_ASYNC_AVAILABLE)
	return -1;
#else /* what about configuring sync? should calling this be an error? */
	return 0;
#endif
}

int odp_ipsec_out_enq(const odp_ipsec_op_param_t *input ODP_UNUSED)
{
	ODP_UNIMPLEMENTED();
#if !(OP_MODE_ASYNC_AVAILABLE)
	return -1;
#else
	return 0;
#endif
}

int odp_ipsec_result(odp_ipsec_op_result_t *result ODP_UNUSED,
		     odp_event_t event ODP_UNUSED)
{
	ODP_UNIMPLEMENTED();
#if !(OP_MODE_ASYNC_AVAILABLE)
	return -1;
#else
	return 0;
#endif
}

int odp_ipsec_mtu_update(odp_ipsec_sa_t sa ODP_UNUSED,
			 uint32_t mtu ODP_UNUSED)
{
	ODP_UNIMPLEMENTED();
	return 0;
}

void *odp_ipsec_sa_context(odp_ipsec_sa_t sa ODP_UNUSED)
{
	ODP_UNIMPLEMENTED();
	return 0;
}
