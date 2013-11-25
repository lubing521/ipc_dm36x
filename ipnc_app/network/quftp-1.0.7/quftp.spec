%define		prefix	/usr

Summary: command line FTP client, with scripting, queueing and server-to-server transfers
Name: quftp
Version: 1.0.7
Release: 1
Source0: quftp-%{version}.tar.gz
Group: applications/internet
Copyright: GPL
URL: http://quftp.sourceforge.net/
Vendor: David Parrish <david@dparrish.com>
Packager: Simon Trimmer <simon@urbanmyth.org>
BuildRoot: /var/tmp/quftp-%{version}.root

%description
Quftp is a command line FTP client, it features:

* an unlimited number of files for upload or download 
* full scripting ability 
* command line completion 
* works with multiple simultaneous FTP servers
* transfers files directly between servers 
* transfers files of any size and type 

%prep
%setup

%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%{prefix}
make 

%install
make DESTDIR="$RPM_BUILD_ROOT" install

%files
%attr(4711,root,root) %{prefix}/bin/quftp
%defattr(-,root,root)
%{prefix}/man/man1/quftp.1.gz
%doc README THANKS TODO example.quftprc example.script quftp.lsm Changes

%clean
rm -rf $RPM_BUILD_ROOT
