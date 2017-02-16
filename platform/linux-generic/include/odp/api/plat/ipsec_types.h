/* Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

/**
 * @file
 *
 * ODP IPSEC API - platform specific types
 */

#ifndef ODP_PLAT_IPSEC_TYPES_H_
#define ODP_PLAT_IPSEC_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <odp/api/std_types.h>
#include <odp/api/plat/strong_types.h>

/** @ingroup odp_ipsec
 *  @{
 */

typedef ODP_HANDLE_T(odp_ipsec_sa_t);

#define ODP_IPSEC_SA_INVALID _odp_cast_scalar(odp_ipsec_sa_t, 0xffffffff)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
