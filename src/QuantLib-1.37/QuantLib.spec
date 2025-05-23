# Define some specific macros
%define _aclocaldir %{_datadir}/aclocal
%define docdir %{_prefix}/doc/QuantLib-%{version}

Summary: The free/open-source library for quantitative finance.
Name: QuantLib
Version: 1.37
Epoch: 0
Release: 0
License: BSD License
Group: System Environment/Libraries
Packager: Liguo Song (Leo) <liguo.song@vanderbilt.edu>
Vendor: QuantLib.org
Source0: http://prdownloads.sourceforge.net/quantlib/QuantLib-%{version}.tar.gz
URL: https://www.quantlib.org/
BuildRoot:%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires: autoconf
BuildRequires: automake
BuildRequires: libtool
BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: rpmlib(FileDigests)
BuildRequires: rpmlib(CompressedFileNames)

%description
QuantLib is an open source C++ library for financial quantitative analysts
and developers.

%package devel
Summary: The header files and the static library.
Group: Development/Libraries
Requires: QuantLib = %{version}, boost >= 1.43.0
BuildRequires: boost-devel >= 1.43.0

%description devel
QuantLib is an open source C++ library for financial quantitative analysts
and developers.

Install QuantLib-devel if you are going to develop programs which will
use the QuantLib C++ libraries.

%package test-suite
Summary: The test-suite to check the setup of quantlib installation.
Group: Development/Tools
Requires: QuantLib = %{version}, boost-test >= 1.43.0
BuildRequires: boost-test >= 1.43.0

%description test-suite
QuantLib is an open source C++ library for financial quantitative analysts
and developers.

The QuantLib-test-suite will validate the compiled code against
pre-constructed test cases, and helps in validating the library.

%package docs
Summary: The documentations for QuantLib.
Group: Documentation
Requires: QuantLib = %{version}
BuildRequires: doxygen >= 1.3, graphviz
Obsoletes: QuantLib-doc

%description docs
QuantLib is an open source C++ library for financial quantitative analysts
and developers.

This package contains documentation files generated from the source code of
QuantLib.

You'll want to install this package if you need a reference to QuantLib.

%prep
%setup -q

%build
CFLAGS="${CFLAGS:-%optflags}" ; export CFLAGS ; \
CXXFLAGS="${CXXFLAGS:-%optflags}" ; export CXXFLAGS ; \
FFLAGS="${FFLAGS:-%optflags}" ; export FFLAGS ; \
%configure --prefix=%{_prefix} EMACS=no
make %{?_smp_mflags}
# make documentations
cd Docs
make docs

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

# install the documentation
mkdir -p %{buildroot}%{docdir}
mkdir -p %{buildroot}%{_mandir}
cp -p LICENSE.TXT ChangeLog.txt Readme.txt %{buildroot}%{docdir}
cp -p Contributors.txt %{buildroot}%{docdir}
cp -p News.txt  %{buildroot}%{docdir}
cp -pr Examples %{buildroot}%{docdir}
cp -pr Docs/html %{buildroot}%{docdir}
rm -f %{buildroot}%{docdir}/html/*.dot

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
%{docdir}/LICENSE.TXT
%{docdir}/ChangeLog.txt
%{docdir}/Readme.txt
%{docdir}/News.txt
%{docdir}/Contributors.txt
%{_libdir}/*

%files devel
%defattr(-,root,root)
%{docdir}/Examples
%{_includedir}/ql/
%{_libdir}/*
%{_datadir}/aclocal/*
%{_bindir}/quantlib-config
%{_mandir}/man1/quantlib-config.1.gz

%files test-suite
%defattr(-,root,root)
%{_mandir}/man1/quantlib-test-suite.1.gz
%{_bindir}/quantlib-test-suite

%files docs
%defattr(-,root,root)
%{docdir}/html

%changelog
* Tue Sep 18 2018 Simon John Rees
- Updated spec file to work with latest QuantLib version (1.13.0)

* Wed Sep 14 2011 Matt Fair
- Updated spec file to work with current QuantLib versions

* Tue Nov 18 2003 Liguo Song <liguo.song@vanderbilt.edu>
- Initial release of 0.3.4
- Rename QuantLib-doc to QuantLib-docs for consistency with the docs tar package

* Thu Oct 2 2003 Liguo Song <liguo.song@vanderbilt.edu>
- Add docs package
- Modified Makefile.am and Makefile.in in Docs/:
    $(DVIPS) refman -> $(DVIPS) -o refman.ps refman

* Fri Aug 22 2003 Liguo Song <liguo.song@vanderbilt.edu>
- Initial package. Refer to Changelog.txt for previous changes.
- RPM package beta test
