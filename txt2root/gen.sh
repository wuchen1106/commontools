#!/bin/bash

#preset
program_file="txt2root.C"
template_file="txt2root_temp.C"
treename="t"
rootfile="out.root"
keyword="\/\/txt2root_preset"

#declare variables
filename=0
num_branch=0
declare -a name_branch

#read file
if [ $# -lt 1 ]; then
  echo You have to identify input file!!!
  exit 1
else
  filename=$1
  echo input file is $filename
fi

sed -n "1 s/\(.*\)/\1/p" $filename > temp
num_branch=`gawk 'BEGIN{RS="  *"};{i++};END{print i}' temp`
echo num_branch = $num_branch
for (( i_branch=1; i_branch<=num_branch; i_branch++ ))
do
  name_branch[i_branch]=`gawk -v gawk_i=$i_branch 'BEGIN{OFS=""};{print $gawk_i}' temp`
  echo ${name_branch[i_branch]}
done

#modify file
echo "  filename = \"$filename\";" > temp
echo "  treename = \"$treename\";" >> temp
echo "  rootfile = \"$rootfile\";" >> temp
echo "  TFile file_output( rootfile.c_str(), \"RECREATE\" );" >> temp
echo "  TTree* d_tree = new TTree( treename.c_str(), treename.c_str() );" >> temp
for (( i_branch=1; i_branch<=num_branch; i_branch++ ))
do
  echo "  double ${name_branch[i_branch]};" >> temp
  echo "  d_tree->Branch(\"${name_branch[i_branch]}\", &${name_branch[i_branch]}, \"${name_branch[i_branch]}/D\");" >> temp
done
echo "  FILE* fin = fopen(filename.c_str(), \"r\");" >> temp
echo "  if(!fin){" >> temp
echo "    std::cout<<\"Cannot find \"<<filename<<std::endl;" >> temp
echo "  }" >> temp
echo "  int iline = 0;" >> temp
echo "  char line[512];" >> temp
echo "  while(fgets(line,sizeof(line),fin)){" >> temp
echo "    if(iline++ == 0) continue;" >> temp
echo "    if(fmod(iline,module) == 0) std::cout<<\"line: \"<<iline<<std::endl;" >> temp
echo -n "    sscanf(line,\"" >> temp
for (( i_branch=1; i_branch<=num_branch; i_branch++ ))
do
  echo -n " %lf" >> temp
done
echo -n "\"" >> temp
for (( i_branch=1; i_branch<=num_branch; i_branch++ ))
do
  echo -n ",&${name_branch[i_branch]}" >> temp
done
echo ");" >>temp
echo "    d_tree->Fill();" >> temp
echo "  }" >> temp

gawk -v gawk_keyword=$keyword 'BEGIN{FS=gawk_keyword;RS="You will not find me, will you?";ORS=""};{print $1}' $template_file > template.1
gawk -v gawk_keyword=$keyword 'BEGIN{FS=gawk_keyword;RS="You will not find me, will you?";ORS=""};{print $3}' $template_file > template.2
cat template.1 temp template.2 > $program_file

#delete temporary files
rm -f temp
rm -f template.1
rm -f template.2
