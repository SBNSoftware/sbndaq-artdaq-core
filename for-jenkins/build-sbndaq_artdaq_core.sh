#!/bin/bash

#parameters

#PRODUCTS=${WORKSPACE}/../products

artdaq_version="v3_08_00"

project_name=sbndaq-artdaq-core
project_url=https://cdcvs.fnal.gov/projects/${project_name}


#main script
usage() {
  cat 1>&2 <<EOF
Usage: $(basename ${0}) [-h]
       $(basename ${0})  <branchtag> <qual_set> <buildtype>
       env WORKSPACE=<workspace> BRANCHTAG=<develop|master|vN_NN_NN> QUAL=<qualifiers> BUILDTYPE=<debug|prof> $(basename ${0})
EOF
}


while getopts :h OPT; do
  case ${OPT} in
    h)
      usage
      exit 1
      ;;
    *)
      usage
      exit 1
  esac
done
shift $(expr $OPTIND - 1)
OPTIND=1


working_dir="${WORKSPACE:-$(pwd)}"
branchtag="${1:-${BRANCHTAG}}"
qual_set="${2:-${QUAL}}"
build_type="${3:-${BUILDTYPE}}"

IFS=':' read -r -a quals <<< "$qual_set"

for onequal in "${quals[@]}"; do
  case ${onequal} in
    e[679]|e1[0-9]|c[0-9])
      basequal=${onequal}
      ;;
    s7[0-9]|s8[0-9]|s9[0-9])
      squal=${onequal}
      ;;
    py2|py3)
      pyqual=${onequal}
      ;;
    debug|prof)
      build_type=${onequal}
      ;;
    *)
      echo "ERROR: unsupported build qualifier \"${onequal}\""
      usage
      exit 1
  esac
done

qual_set="${squal}:${basequal}"
[[ ! -z "${pyqual+x}" ]] && qual_set="${qual_set}:${pyqual}"

case ${build_type} in
  debug) build_type_flag="-d" ;;
  prof) build_type_flag="-p" ;;
  *)
    echo "ERROR: build type must be debug or prof"
    usage
    exit 1
esac



# Find platform flavor.
OS=$(uname)
if [ "${OS}" = "Linux" ]
then
  id=$(lsb_release -is)
  if [ "${id}" = "Ubuntu" ]
  then
    flvr=u$(lsb_release -r | sed -e 's/[[:space:]]//g' | cut -f2 -d":" | cut -f1 -d".")
  else
    flvr=slf$(lsb_release -r | sed -e 's/[[:space:]]//g' | cut -f2 -d":" | cut -f1 -d".")
  fi
else
  echo "ERROR: unrecognized operating system ${OS}"
  exit 1
fi


echo "build variables"
echo " working_dir=${working_dir}"
echo " branchtag=${branchtag}"
echo " qual_set=${qual_set}"
echo " build_type=${build_type}"
echo " flvr=${flvr}"


#set -x

product_name=${project_name//-/_}

src_dir=${working_dir}/source
build_dir=${working_dir}/build
products_dir=${working_dir}/products
copyback_dir=${working_dir}/copyBack



# start with clean directories
(cd "${working_dir}"; ls -l; du -sh *)
rm -rf ${build_dir}
rm -rf ${src_dir}
rm -rf ${copyback_dir}
# now make the dfirectories
mkdir -p ${src_dir} || exit 1
mkdir -p ${build_dir} || exit 1
mkdir -p ${copyback_dir} || exit 1
(cd "${working_dir}"; ls -l; du -sh *)



echo
echo "checkout source"
echo
cd ${src_dir} || exit 1
ls -la
git clone ${project_url} ${product_name}
cd ${product_name} || exit 1
git checkout ${branchtag}



echo
echo "pull products from scisoft"
echo
#unset PRODUCTS
[[ ! -d ${products_dir} ]] && mkdir -p ${products_dir}
cd ${products_dir} || exit 1
curl --fail --silent --location --insecure -O http://scisoft.fnal.gov/scisoft/bundles/tools/pullProducts || exit 1
chmod +x pullProducts
./pullProducts ${products_dir} ${flvr} artdaq-${artdaq_version} ${qual_set//:/-} ${build_type} 2>&1 |tee ${products_dir}/pullproducts.log



echo
product_version=$(cat  ${src_dir}/${product_name}/ups/product_deps | grep -E "^parent.*${product_name}.*v.*"| grep -Eo "v[0-9]_[0-9]{2}_[0-9]{2}.*")
echo "building the ${product_name} distribution for ${product_version} ${qual_set} ${build_type}"
echo
[[ -d ${build_dir} ]] && rm -rf ${build_dir}
[[ ! -d ${build_dir} ]] && mkdir -p ${build_dir}
cd ${build_dir} || exit 1
source ${products_dir}/setup || exit 1
source ${src_dir}/${product_name}/ups/setup_for_development  ${build_type_flag}  ${qual_set//:/ }
export MRB_QUALS="${qual_set}:${build_type}"
previnstall_dir=${products_dir}/${product_name}/${product_version}
[[ -d ${previnstall_dir} ]] && rm -rf ${previnstall_dir}{,version}

buildtool -I "${products_dir}" -pi -j$(nproc) 2>&1 | tee ${build_dir}/build-${product_name}.log || \
 { mv ${build_dir}/*.log  ${copyback_dir}/
   exit 1
 }

buildtool -t -j$(nproc) 2>&1 | tee ${build_dir}/test-${product_name}.log || \
 { mv ${build_dir}/*.log  ${copyback_dir}/
   exit 1
 }



echo
echo "move files"
echo
mv ${build_dir}/{*.bz2,*.log}  ${copyback_dir}/
mv ${products_dir}/*.log  ${copyback_dir}/



echo
echo "cleanup"
echo
rm -rf "${build_dir}"
rm -rf "${src_dir}"

exit 0
