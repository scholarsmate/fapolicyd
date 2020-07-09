%module fapolicyanalyzer
%{
#include "libfapolicyd-analyzer/error_codes.h"
#include "libfapolicyd-analyzer/match.h"
#include "libfapolicyd-analyzer/sha256.h"
#include "libfapolicyd-analyzer/system_trust.hpp"
#include "libfapolicyd-analyzer/table.hpp"
%}

%include "libfapolicyd-analyzer/error_codes.h"
%include "libfapolicyd-analyzer/match.h"
%include "libfapolicyd-analyzer/sha256.h"
%include "libfapolicyd-analyzer/system_trust.hpp"
%include "libfapolicyd-analyzer/table.hpp"
