#!/bin/bash
scratch_dir=${HOME}/tmp

function publishscisoft(){
  local ts=$(date -d "today" +"%Y%m%d%H%M%S")
  local tmp_packages="${scratch_dir}/publish-${ts}"

  mkdir -p ${tmp_packages} || exit 1
  cd  ${tmp_packages}

  curl -O https://scisoft.fnal.gov/scisoft/bundles/tools/copyFromJenkins
  chmod a+x copyFromJenkins

  curl -O https://scisoft.fnal.gov/scisoft/bundles/tools/copyToSciSoft
  chmod a+x copyToSciSoft

 ./copyFromJenkins \
-m SLF7 \
-q s106-c7  \
-q s106-e19 \
-q s106-e20 \
-q s108-c7  \
-q s108-e19 \
-q s108-e20 \
-q s109-c7  \
-q s109-e19 \
-q s109-e20 \
-q s110-c7  \
-q s110-e19 \
-q s110-e20 \
-q s111-c7  \
-q s111-e19 \
-q s111-e20 \
 sbndaq_artdaq_core-build


find ./ -name "*bz2" |xargs -n 1 -I {} ./copyToSciSoft {}
}

publishscisoft
