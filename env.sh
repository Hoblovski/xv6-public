# A collection of cryptic shorthands
r0() {
  if [[ -z $KW ]]
  then
    echo set KW first
  else
    sed "/\[$KW\]/!d; s/.*\[$KW\] \(0x\S*\) \(0x\S*\).*/\1 \2/" $@
  fi
}
alias r1="awk '{print strtonum(\$1)*(2**32)+strtonum(\$2)}'"
alias r2="awk '{print \$1-x; x=\$1}'"
alias r2s="awk '{if(NR%2==0) print \$0-x; x=\$0}'"
alias r3="python3 count.py"
