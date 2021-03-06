/*! \file */
/* ************************************************************************
 * Copyright (c) 2018-2020 Advanced Micro Devices, Inc.
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

#include "definitions.h"
#include "rocsparse_csrsv.hpp"
#include "utility.h"
#include <rocprim/rocprim.hpp>

template <typename T>
rocsparse_status rocsparse_csrsv_buffer_size_template(rocsparse_handle          handle,
                                                      rocsparse_operation       trans,
                                                      rocsparse_int             m,
                                                      rocsparse_int             nnz,
                                                      const rocsparse_mat_descr descr,
                                                      const T*                  csr_val,
                                                      const rocsparse_int*      csr_row_ptr,
                                                      const rocsparse_int*      csr_col_ind,
                                                      rocsparse_mat_info        info,
                                                      size_t*                   buffer_size)
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
    else if(info == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Logging
    log_trace(handle,
              replaceX<T>("rocsparse_Xcsrsv_buffer_size"),
              trans,
              m,
              nnz,
              (const void*&)descr,
              (const void*&)csr_val,
              (const void*&)csr_row_ptr,
              (const void*&)csr_col_ind,
              (const void*&)info,
              (const void*&)buffer_size);

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

    // Check operation type
    if(trans != rocsparse_operation_none && trans != rocsparse_operation_transpose)
    {
        return rocsparse_status_not_implemented;
    }

    // Check sizes
    if(m < 0)
    {
        return rocsparse_status_invalid_size;
    }
    else if(nnz < 0)
    {
        return rocsparse_status_invalid_size;
    }

    // Check for valid buffer_size pointer
    if(buffer_size == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Quick return if possible
    if(m == 0 || nnz == 0)
    {
        // Do not return 0 as buffer size
        *buffer_size = 4;
        return rocsparse_status_success;
    }

    // Check pointer arguments
    if(csr_row_ptr == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }
    else if(csr_col_ind == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }
    else if(csr_val == nullptr)
    {
        return rocsparse_status_invalid_pointer;
    }

    // Stream
    hipStream_t stream = handle->stream;

    // rocsparse_int max_nnz
    *buffer_size = 256;

    // rocsparse_int done_array[m]
    *buffer_size += sizeof(int) * ((m - 1) / 256 + 1) * 256;

    // rocsparse_int workspace
    *buffer_size += sizeof(rocsparse_int) * ((m - 1) / 256 + 1) * 256;

    // rocsparse_int workspace2
    *buffer_size += sizeof(int) * ((m - 1) / 256 + 1) * 256;

    size_t         rocprim_size = 0;
    rocsparse_int* ptr          = reinterpret_cast<rocsparse_int*>(buffer_size);
    int*           ptr2         = reinterpret_cast<int*>(buffer_size);

    rocprim::double_buffer<rocsparse_int> dummy(ptr, ptr);
    rocprim::double_buffer<int>           dummy2(ptr2, ptr2);

    RETURN_IF_HIP_ERROR(
        rocprim::radix_sort_pairs(nullptr, rocprim_size, dummy2, dummy, m, 0, 32, stream));

    // rocprim buffer
    *buffer_size += rocprim_size;

    // On transposed case, we might need more temporary storage for transposing
    if(trans == rocsparse_operation_transpose)
    {
        size_t transpose_size;

        // Determine rocprim buffer size
        RETURN_IF_HIP_ERROR(
            rocprim::radix_sort_pairs(nullptr, transpose_size, dummy, dummy, nnz, 0, 32, stream));

        // rocPRIM does not support in-place sorting, so we need an additional buffer
        transpose_size += sizeof(rocsparse_int) * ((nnz - 1) / 256 + 1) * 256;
        transpose_size += sizeof(T) * ((nnz - 1) / 256 + 1) * 256;

        *buffer_size = std::max(*buffer_size, transpose_size);
    }

    return rocsparse_status_success;
}

/*
 * ===========================================================================
 *    C wrapper
 * ===========================================================================
 */
#define C_IMPL(NAME, TYPE)                                                                       \
    extern "C" rocsparse_status NAME(rocsparse_handle          handle,                           \
                                     rocsparse_operation       trans,                            \
                                     rocsparse_int             m,                                \
                                     rocsparse_int             nnz,                              \
                                     const rocsparse_mat_descr descr,                            \
                                     const TYPE*               csr_val,                          \
                                     const rocsparse_int*      csr_row_ptr,                      \
                                     const rocsparse_int*      csr_col_ind,                      \
                                     rocsparse_mat_info        info,                             \
                                     size_t*                   buffer_size)                      \
    {                                                                                            \
        return rocsparse_csrsv_buffer_size_template(                                             \
            handle, trans, m, nnz, descr, csr_val, csr_row_ptr, csr_col_ind, info, buffer_size); \
    }

C_IMPL(rocsparse_scsrsv_buffer_size, float);
C_IMPL(rocsparse_dcsrsv_buffer_size, double);
C_IMPL(rocsparse_ccsrsv_buffer_size, rocsparse_float_complex);
C_IMPL(rocsparse_zcsrsv_buffer_size, rocsparse_double_complex);

#undef C_IMPL
