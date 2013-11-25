## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: linker.cmd

linker.cmd: \
  package/cfg/alg_server_x470MV.o470MV \
  package/cfg/alg_server_x470MV.xdl
	$(SED) 's"^\"\(package/cfg/alg_server_x470MVcfg.cmd\)\"$""\"/home/jiangjx/UbuntuShare/ipnc_project/ipnc_lhy/github_ipc_lhy/av_capture/framework/alg/src/alg_config/\1\""' package/cfg/alg_server_x470MV.xdl > $@
