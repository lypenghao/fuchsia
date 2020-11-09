// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package codegen

const sourceFileTmpl = `
{{- define "GenerateSourceFile" -}}
// WARNING: This file is machine generated by fidlgen.

#![allow(
	deprecated, // FIDL Impl struct pattern is referenced internally
	unused_parens, // one-element-tuple-case is not a tuple
	unused_mut, // not all args require mutation, but many do
	nonstandard_style, // auto-caps does its best, but is not always successful
)]

#![recursion_limit="512"]

#[cfg(target_os = "fuchsia")]
#[allow(unused_imports)]
use fuchsia_zircon as zx;

#[allow(unused_imports)]
use {
	bitflags::bitflags,
	fuchsia_zircon_status as zx_status,
  	futures::future::{self, MaybeDone, TryFutureExt},
	fidl::{
		fidl_empty_struct,
		fidl_flexible_bits,
		fidl_flexible_enum,
		fidl_strict_bits,
		fidl_strict_enum,
		fidl_struct_copy,
		fidl_struct,
		fidl_table,
		fidl_xunion,
		wrap_handle_metadata,
		client::{
			QueryResponseFut,
			decode_transaction_body_fut,
		},
	},
};

{{/*
	For FIDL tracing, the Rust bindings are assigned a bindings enum of 6. See
	fidl-trace/trace_impl.cc, enum LogContext for more context.

	This constant will be unused in a build that has FIDL tracing disabled,
	since this constant is used only within trace_duration! macros that will
	expand to no-ops. However, the constant needs to be present, since the
	constant is evaluated before the macro expansion.

	The constant starts with an underscore to avoid clashing with FIDL
	declarations, which are not allowed to start with underscores.
*/ -}}
{{ if .Protocols -}}
const _FIDL_TRACE_BINDINGS_RUST: u32 = 6;
{{ end -}}

{{ range $bits := .Bits -}}
{{ template "BitsDeclaration" $bits }}
{{ end -}}
{{ range $const := .Consts -}}
{{ template "ConstDeclaration" $const }}
{{ end -}}
{{ range $enum := .Enums -}}
{{ template "EnumDeclaration" $enum }}
{{ end -}}
{{ range $result := .Results -}}
{{ template "ResultDeclaration" $result}}
{{ end -}}
{{ range $union := .Unions -}}
{{ template "UnionDeclaration" $union }}
{{ end -}}
{{ range $struct := .Structs -}}
{{ template "StructDeclaration" $struct }}
{{ end -}}
{{ range $table := .Tables -}}
{{ template "TableDeclaration" $table }}
{{ end -}}
{{ range $hmw := .HandleMetadataWrappers -}}
{{ template "HandleMetadataWrapperDeclaration" $hmw }}
{{ end -}}
{{ range $protocol := .Protocols -}}
{{ range $transport, $_ := .Transports -}}
{{ if eq $transport "Channel" -}}{{ template "ProtocolDeclaration" $protocol }}{{- end }}
{{ end -}}
{{ end -}}
{{ range $service := .Services -}}
{{ range $transport, $_ := .Transports -}}
{{ if eq $transport "Channel" -}}{{ template "ServiceDeclaration" $service }}{{- end }}
{{ end -}}
{{ end -}}
{{- end -}}
`
