/* ************************************************************************
 * Copyright (c) 2020-2021 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * ************************************************************************ */

#include "rocsparse_gebsrmv.hpp"
#include "rocsparse.h"
#include "rocsparse_bsrmv.hpp"
#include "rocsparse_csrmv.hpp"

#include "definitions.h"
#include "gebsrmv_device.h"
#include "handle.h"
#include "rocsparse.h"
#include "utility.h"

#include <hip/hip_runtime.h>

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmv_template_row_block_dim_1(rocsparse_handle          handle,
                                                            rocsparse_direction       dir,
                                                            rocsparse_operation       trans,
                                                            rocsparse_int             mb,
                                                            rocsparse_int             nb,
                                                            rocsparse_int             nnzb,
                                                            U                         alpha,
                                                            const rocsparse_mat_descr descr,
                                                            const T*                  bsr_val,
                                                            const rocsparse_int*      bsr_row_ptr,
                                                            const rocsparse_int*      bsr_col_ind,
                                                            rocsparse_int             row_block_dim,
                                                            rocsparse_int             col_block_dim,
                                                            const T*                  x,
                                                            U                         beta,
                                                            T*                        y);

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmv_template_row_block_dim_2(rocsparse_handle          handle,
                                                            rocsparse_direction       dir,
                                                            rocsparse_operation       trans,
                                                            rocsparse_int             mb,
                                                            rocsparse_int             nb,
                                                            rocsparse_int             nnzb,
                                                            U                         alpha,
                                                            const rocsparse_mat_descr descr,
                                                            const T*                  bsr_val,
                                                            const rocsparse_int*      bsr_row_ptr,
                                                            const rocsparse_int*      bsr_col_ind,
                                                            rocsparse_int             row_block_dim,
                                                            rocsparse_int             col_block_dim,
                                                            const T*                  x,
                                                            U                         beta,
                                                            T*                        y);

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmv_template_row_block_dim_3(rocsparse_handle          handle,
                                                            rocsparse_direction       dir,
                                                            rocsparse_operation       trans,
                                                            rocsparse_int             mb,
                                                            rocsparse_int             nb,
                                                            rocsparse_int             nnzb,
                                                            U                         alpha,
                                                            const rocsparse_mat_descr descr,
                                                            const T*                  bsr_val,
                                                            const rocsparse_int*      bsr_row_ptr,
                                                            const rocsparse_int*      bsr_col_ind,
                                                            rocsparse_int             row_block_dim,
                                                            rocsparse_int             col_block_dim,
                                                            const T*                  x,
                                                            U                         beta,
                                                            T*                        y);

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmv_template_row_block_dim_4(rocsparse_handle          handle,
                                                            rocsparse_direction       dir,
                                                            rocsparse_operation       trans,
                                                            rocsparse_int             mb,
                                                            rocsparse_int             nb,
                                                            rocsparse_int             nnzb,
                                                            U                         alpha,
                                                            const rocsparse_mat_descr descr,
                                                            const T*                  bsr_val,
                                                            const rocsparse_int*      bsr_row_ptr,
                                                            const rocsparse_int*      bsr_col_ind,
                                                            rocsparse_int             row_block_dim,
                                                            rocsparse_int             col_block_dim,
                                                            const T*                  x,
                                                            U                         beta,
                                                            T*                        y);

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmv_template_row_block_dim_5_8(rocsparse_handle          handle,
                                                              rocsparse_direction       dir,
                                                              rocsparse_operation       trans,
                                                              rocsparse_int             mb,
                                                              rocsparse_int             nb,
                                                              rocsparse_int             nnzb,
                                                              U                         alpha,
                                                              const rocsparse_mat_descr descr,
                                                              const T*                  bsr_val,
                                                              const rocsparse_int*      bsr_row_ptr,
                                                              const rocsparse_int*      bsr_col_ind,
                                                              rocsparse_int row_block_dim,
                                                              rocsparse_int col_block_dim,
                                                              const T*      x,
                                                              U             beta,
                                                              T*            y);

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmv_template_row_block_dim_9_12(rocsparse_handle          handle,
                                                               rocsparse_direction       dir,
                                                               rocsparse_operation       trans,
                                                               rocsparse_int             mb,
                                                               rocsparse_int             nb,
                                                               rocsparse_int             nnzb,
                                                               U                         alpha,
                                                               const rocsparse_mat_descr descr,
                                                               const T*                  bsr_val,
                                                               const rocsparse_int* bsr_row_ptr,
                                                               const rocsparse_int* bsr_col_ind,
                                                               rocsparse_int        row_block_dim,
                                                               rocsparse_int        col_block_dim,
                                                               const T*             x,
                                                               U                    beta,
                                                               T*                   y);

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmv_template_row_block_dim_13_16(rocsparse_handle          handle,
                                                                rocsparse_direction       dir,
                                                                rocsparse_operation       trans,
                                                                rocsparse_int             mb,
                                                                rocsparse_int             nb,
                                                                rocsparse_int             nnzb,
                                                                U                         alpha,
                                                                const rocsparse_mat_descr descr,
                                                                const T*                  bsr_val,
                                                                const rocsparse_int* bsr_row_ptr,
                                                                const rocsparse_int* bsr_col_ind,
                                                                rocsparse_int        row_block_dim,
                                                                rocsparse_int        col_block_dim,
                                                                const T*             x,
                                                                U                    beta,
                                                                T*                   y);

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmv_template_row_block_dim_17_inf(rocsparse_handle          handle,
                                                                 rocsparse_direction       dir,
                                                                 rocsparse_operation       trans,
                                                                 rocsparse_int             mb,
                                                                 rocsparse_int             nb,
                                                                 rocsparse_int             nnzb,
                                                                 U                         alpha,
                                                                 const rocsparse_mat_descr descr,
                                                                 const T*                  bsr_val,
                                                                 const rocsparse_int* bsr_row_ptr,
                                                                 const rocsparse_int* bsr_col_ind,
                                                                 rocsparse_int        row_block_dim,
                                                                 rocsparse_int        col_block_dim,
                                                                 const T*             x,
                                                                 U                    beta,
                                                                 T*                   y);

template <typename T, typename U>
rocsparse_status rocsparse_gebsrmv_template_dispatch(rocsparse_handle          handle,
                                                     rocsparse_direction       dir,
                                                     rocsparse_operation       trans,
                                                     rocsparse_int             mb,
                                                     rocsparse_int             nb,
                                                     rocsparse_int             nnzb,
                                                     U                         alpha,
                                                     const rocsparse_mat_descr descr,
                                                     const T*                  bsr_val,
                                                     const rocsparse_int*      bsr_row_ptr,
                                                     const rocsparse_int*      bsr_col_ind,
                                                     rocsparse_int             row_block_dim,
                                                     rocsparse_int             col_block_dim,
                                                     const T*                  x,
                                                     U                         beta,
                                                     T*                        y)
{

    // row_block_dim == col_block_dim is the BSR case
    if(row_block_dim == col_block_dim)
    {
        RETURN_IF_ROCSPARSE_ERROR(rocsparse_bsrmv_template_dispatch(handle,
                                                                    dir,
                                                                    trans,
                                                                    mb,
                                                                    nb,
                                                                    nnzb,
                                                                    alpha,
                                                                    descr,
                                                                    bsr_val,
                                                                    bsr_row_ptr,
                                                                    bsr_col_ind,
                                                                    row_block_dim,
                                                                    x,
                                                                    beta,
                                                                    y));

        return rocsparse_status_success;
    }

    if(row_block_dim == 1)
    {
        RETURN_IF_ROCSPARSE_ERROR(rocsparse_gebsrmv_template_row_block_dim_1(handle,
                                                                             dir,
                                                                             trans,
                                                                             mb,
                                                                             nb,
                                                                             nnzb,
                                                                             alpha,
                                                                             descr,
                                                                             bsr_val,
                                                                             bsr_row_ptr,
                                                                             bsr_col_ind,
                                                                             row_block_dim,
                                                                             col_block_dim,
                                                                             x,
                                                                             beta,
                                                                             y));
    }
    else if(row_block_dim == 2)
    {
        RETURN_IF_ROCSPARSE_ERROR(rocsparse_gebsrmv_template_row_block_dim_2(handle,
                                                                             dir,
                                                                             trans,
                                                                             mb,
                                                                             nb,
                                                                             nnzb,
                                                                             alpha,
                                                                             descr,
                                                                             bsr_val,
                                                                             bsr_row_ptr,
                                                                             bsr_col_ind,
                                                                             row_block_dim,
                                                                             col_block_dim,
                                                                             x,
                                                                             beta,
                                                                             y));
    }
    else if(row_block_dim == 3)
    {
        RETURN_IF_ROCSPARSE_ERROR(rocsparse_gebsrmv_template_row_block_dim_3(handle,
                                                                             dir,
                                                                             trans,
                                                                             mb,
                                                                             nb,
                                                                             nnzb,
                                                                             alpha,
                                                                             descr,
                                                                             bsr_val,
                                                                             bsr_row_ptr,
                                                                             bsr_col_ind,
                                                                             row_block_dim,
                                                                             col_block_dim,
                                                                             x,
                                                                             beta,
                                                                             y));
    }
    else if(row_block_dim == 4)
    {
        RETURN_IF_ROCSPARSE_ERROR(rocsparse_gebsrmv_template_row_block_dim_4(handle,
                                                                             dir,
                                                                             trans,
                                                                             mb,
                                                                             nb,
                                                                             nnzb,
                                                                             alpha,
                                                                             descr,
                                                                             bsr_val,
                                                                             bsr_row_ptr,
                                                                             bsr_col_ind,
                                                                             row_block_dim,
                                                                             col_block_dim,
                                                                             x,
                                                                             beta,
                                                                             y));
    }
    else if(row_block_dim <= 8)
    {
        RETURN_IF_ROCSPARSE_ERROR(rocsparse_gebsrmv_template_row_block_dim_5_8(handle,
                                                                               dir,
                                                                               trans,
                                                                               mb,
                                                                               nb,
                                                                               nnzb,
                                                                               alpha,
                                                                               descr,
                                                                               bsr_val,
                                                                               bsr_row_ptr,
                                                                               bsr_col_ind,
                                                                               row_block_dim,
                                                                               col_block_dim,
                                                                               x,
                                                                               beta,
                                                                               y));

        return rocsparse_status_success;
    }
    else if(row_block_dim <= 12)
    {
        RETURN_IF_ROCSPARSE_ERROR(rocsparse_gebsrmv_template_row_block_dim_9_12(handle,
                                                                                dir,
                                                                                trans,
                                                                                mb,
                                                                                nb,
                                                                                nnzb,
                                                                                alpha,
                                                                                descr,
                                                                                bsr_val,
                                                                                bsr_row_ptr,
                                                                                bsr_col_ind,
                                                                                row_block_dim,
                                                                                col_block_dim,
                                                                                x,
                                                                                beta,
                                                                                y));

        return rocsparse_status_success;
    }
    else if(row_block_dim <= 16)
    {
        RETURN_IF_ROCSPARSE_ERROR(rocsparse_gebsrmv_template_row_block_dim_13_16(handle,
                                                                                 dir,
                                                                                 trans,
                                                                                 mb,
                                                                                 nb,
                                                                                 nnzb,
                                                                                 alpha,
                                                                                 descr,
                                                                                 bsr_val,
                                                                                 bsr_row_ptr,
                                                                                 bsr_col_ind,
                                                                                 row_block_dim,
                                                                                 col_block_dim,
                                                                                 x,
                                                                                 beta,
                                                                                 y));

        return rocsparse_status_success;
    }
    else
    {
        RETURN_IF_ROCSPARSE_ERROR(rocsparse_gebsrmv_template_row_block_dim_17_inf(handle,
                                                                                  dir,
                                                                                  trans,
                                                                                  mb,
                                                                                  nb,
                                                                                  nnzb,
                                                                                  alpha,
                                                                                  descr,
                                                                                  bsr_val,
                                                                                  bsr_row_ptr,
                                                                                  bsr_col_ind,
                                                                                  row_block_dim,
                                                                                  col_block_dim,
                                                                                  x,
                                                                                  beta,
                                                                                  y));

        return rocsparse_status_success;
    }

    return rocsparse_status_success;
}

template <typename T>
rocsparse_status rocsparse_gebsrmv_template(rocsparse_handle          handle,
                                            rocsparse_direction       dir,
                                            rocsparse_operation       trans,
                                            rocsparse_int             mb,
                                            rocsparse_int             nb,
                                            rocsparse_int             nnzb,
                                            const T*                  alpha,
                                            const rocsparse_mat_descr descr,
                                            const T*                  bsr_val,
                                            const rocsparse_int*      bsr_row_ptr,
                                            const rocsparse_int*      bsr_col_ind,
                                            rocsparse_int             row_block_dim,
                                            rocsparse_int             col_block_dim,
                                            const T*                  x,
                                            const T*                  beta,
                                            T*                        y)
{
    // Check for valid handle and matrix descriptor
    if(handle == nullptr)
    {
        return rocsparse_status_invalid_handle;
    }
    else if(descr == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Logging
    if(handle->pointer_mode == rocsparse_pointer_mode_host)
    {
        log_trace(handle,
                  replaceX<T>("rocsparse_Xgebsrmv"),
                  dir,
                  trans,
                  mb,
                  nb,
                  nnzb,
                  *alpha,
                  (const void*&)descr,
                  (const void*&)bsr_val,
                  (const void*&)bsr_row_ptr,
                  (const void*&)bsr_col_ind,
                  row_block_dim,
                  col_block_dim,
                  (const void*&)x,
                  *beta,
                  (const void*&)y);

        log_bench(handle,
                  "./rocsparse-bench -f gebsrmv -r",
                  replaceX<T>("X"),
                  "--mtx <matrix.mtx> "
                  "--rblockdim",
                  row_block_dim,
                  "--cblockdim",
                  col_block_dim,
                  "--alpha",
                  *alpha,
                  "--beta",
                  *beta);
    }
    else
    {
        log_trace(handle,
                  replaceX<T>("rocsparse_Xgebsrmv"),
                  dir,
                  trans,
                  mb,
                  nb,
                  nnzb,
                  (const void*&)alpha,
                  (const void*&)descr,
                  (const void*&)bsr_val,
                  (const void*&)bsr_row_ptr,
                  (const void*&)bsr_col_ind,
                  row_block_dim,
                  col_block_dim,
                  (const void*&)x,
                  (const void*&)beta,
                  (const void*&)y);
    }

    // Check index base
    if(descr->base != rocsparse_index_base_zero && descr->base != rocsparse_index_base_one)
    {
        return rocsparse_status_invalid_value;
    }
    if(descr->type != rocsparse_matrix_type_general)
    {
        // TODO
        return rocsparse_status_not_implemented;
    }

    // Check sizes
    if(mb < 0)
    {
        return rocsparse_status_invalid_size;
    }
    else if(nb < 0)
    {
        return rocsparse_status_invalid_size;
    }
    else if(nnzb < 0)
    {
        return rocsparse_status_invalid_size;
    }
    else if(row_block_dim < 0 || col_block_dim < 0)
    {
        return rocsparse_status_invalid_size;
    }

    // Quick return if possible
    if(mb == 0 || nb == 0 || nnzb == 0 || row_block_dim == 0 || col_block_dim == 0)
    {
        return rocsparse_status_success;
    }

    // Check pointer arguments
    if(bsr_val == nullptr || bsr_row_ptr == nullptr || bsr_col_ind == nullptr || x == nullptr
       || y == nullptr || alpha == nullptr || beta == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // row_block_dim == 1 and col_block_dim == 1 is the CSR case
    if(row_block_dim == 1 && col_block_dim == 1)
    {
        RETURN_IF_ROCSPARSE_ERROR(rocsparse_csrmv_template(handle,
                                                           trans,
                                                           mb,
                                                           nb,
                                                           nnzb,
                                                           alpha,
                                                           descr,
                                                           bsr_val,
                                                           bsr_row_ptr,
                                                           bsr_col_ind,
                                                           nullptr,
                                                           x,
                                                           beta,
                                                           y));

        return rocsparse_status_success;
    }

    // Run different gebsrmv kernels
    if(handle->pointer_mode == rocsparse_pointer_mode_device)
    {
        return rocsparse_gebsrmv_template_dispatch(handle,
                                                   dir,
                                                   trans,
                                                   mb,
                                                   nb,
                                                   nnzb,
                                                   alpha,
                                                   descr,
                                                   bsr_val,
                                                   bsr_row_ptr,
                                                   bsr_col_ind,
                                                   row_block_dim,
                                                   col_block_dim,
                                                   x,
                                                   beta,
                                                   y);
    }
    else
    {
        return rocsparse_gebsrmv_template_dispatch(handle,
                                                   dir,
                                                   trans,
                                                   mb,
                                                   nb,
                                                   nnzb,
                                                   *alpha,
                                                   descr,
                                                   bsr_val,
                                                   bsr_row_ptr,
                                                   bsr_col_ind,
                                                   row_block_dim,
                                                   col_block_dim,
                                                   x,
                                                   *beta,
                                                   y);
    }
    return rocsparse_status_success;
}

/*
 * ===========================================================================
 *    C wrapper
 * ===========================================================================
 */

extern "C" rocsparse_status rocsparse_sgebsrmv(rocsparse_handle          handle,
                                               rocsparse_direction       dir,
                                               rocsparse_operation       trans,
                                               rocsparse_int             mb,
                                               rocsparse_int             nb,
                                               rocsparse_int             nnzb,
                                               const float*              alpha,
                                               const rocsparse_mat_descr descr,
                                               const float*              bsr_val,
                                               const rocsparse_int*      bsr_row_ptr,
                                               const rocsparse_int*      bsr_col_ind,
                                               rocsparse_int             row_block_dim,
                                               rocsparse_int             col_block_dim,
                                               const float*              x,
                                               const float*              beta,
                                               float*                    y)
{
    return rocsparse_gebsrmv_template(handle,
                                      dir,
                                      trans,
                                      mb,
                                      nb,
                                      nnzb,
                                      alpha,
                                      descr,
                                      bsr_val,
                                      bsr_row_ptr,
                                      bsr_col_ind,
                                      row_block_dim,
                                      col_block_dim,
                                      x,
                                      beta,
                                      y);
}

extern "C" rocsparse_status rocsparse_dgebsrmv(rocsparse_handle          handle,
                                               rocsparse_direction       dir,
                                               rocsparse_operation       trans,
                                               rocsparse_int             mb,
                                               rocsparse_int             nb,
                                               rocsparse_int             nnzb,
                                               const double*             alpha,
                                               const rocsparse_mat_descr descr,
                                               const double*             bsr_val,
                                               const rocsparse_int*      bsr_row_ptr,
                                               const rocsparse_int*      bsr_col_ind,
                                               rocsparse_int             row_block_dim,
                                               rocsparse_int             col_block_dim,
                                               const double*             x,
                                               const double*             beta,
                                               double*                   y)
{
    return rocsparse_gebsrmv_template(handle,
                                      dir,
                                      trans,
                                      mb,
                                      nb,
                                      nnzb,
                                      alpha,
                                      descr,
                                      bsr_val,
                                      bsr_row_ptr,
                                      bsr_col_ind,
                                      row_block_dim,
                                      col_block_dim,
                                      x,
                                      beta,
                                      y);
}

extern "C" rocsparse_status rocsparse_cgebsrmv(rocsparse_handle               handle,
                                               rocsparse_direction            dir,
                                               rocsparse_operation            trans,
                                               rocsparse_int                  mb,
                                               rocsparse_int                  nb,
                                               rocsparse_int                  nnzb,
                                               const rocsparse_float_complex* alpha,
                                               const rocsparse_mat_descr      descr,
                                               const rocsparse_float_complex* bsr_val,
                                               const rocsparse_int*           bsr_row_ptr,
                                               const rocsparse_int*           bsr_col_ind,
                                               rocsparse_int                  row_block_dim,
                                               rocsparse_int                  col_block_dim,
                                               const rocsparse_float_complex* x,
                                               const rocsparse_float_complex* beta,
                                               rocsparse_float_complex*       y)
{
    return rocsparse_gebsrmv_template(handle,
                                      dir,
                                      trans,
                                      mb,
                                      nb,
                                      nnzb,
                                      alpha,
                                      descr,
                                      bsr_val,
                                      bsr_row_ptr,
                                      bsr_col_ind,
                                      row_block_dim,
                                      col_block_dim,
                                      x,
                                      beta,
                                      y);
}

extern "C" rocsparse_status rocsparse_zgebsrmv(rocsparse_handle                handle,
                                               rocsparse_direction             dir,
                                               rocsparse_operation             trans,
                                               rocsparse_int                   mb,
                                               rocsparse_int                   nb,
                                               rocsparse_int                   nnzb,
                                               const rocsparse_double_complex* alpha,
                                               const rocsparse_mat_descr       descr,
                                               const rocsparse_double_complex* bsr_val,
                                               const rocsparse_int*            bsr_row_ptr,
                                               const rocsparse_int*            bsr_col_ind,
                                               rocsparse_int                   row_block_dim,
                                               rocsparse_int                   col_block_dim,
                                               const rocsparse_double_complex* x,
                                               const rocsparse_double_complex* beta,
                                               rocsparse_double_complex*       y)
{
    return rocsparse_gebsrmv_template(handle,
                                      dir,
                                      trans,
                                      mb,
                                      nb,
                                      nnzb,
                                      alpha,
                                      descr,
                                      bsr_val,
                                      bsr_row_ptr,
                                      bsr_col_ind,
                                      row_block_dim,
                                      col_block_dim,
                                      x,
                                      beta,
                                      y);
}
