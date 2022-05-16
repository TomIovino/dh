mixed *syllables;

void reset(int arg) {
    if(arg) return;
    syllables = allocate(3);
    syllables[0] = ({"Ral","Na","Ard","Vald","Cal","Hy","Pan","Chies","Per",
                     "Er","Hob","Harg","Win","Mar","Quarne","Ba","Er","Odas",
                     "Ka","Mold","Syn","Ro","Jast","Yal","Nap","Vard","As",
                     "Binthe","Zald","Dez","Las","Uld","Nev","Haur","Bar",
                     "Das","Ty","Dar","Ost","Tral","Grave","Eth","Flar","Yal",
                     "Klab","Harab","Jar","Nor","Dain","Toc","Bay","Haith",
                     "Cal","Lar","Naut","Druc","Bar","Art","For","Mart","Yar",
                     "Ha","Ny","Yar","Verd","Wy","Plag","Ter","Haur","Var",
                     "Ar","Dar","Val","Mar","Car","Loc","Wearn","Dras","Bel",
                     "Har","Jar","For","Kil","Oc","Al","Skal","Nun","Az",
                     "Kop","Houl","Lab","Jar","Vast","Claune","Tes","Ob",
                     "Nist","El","Est","Zol","Brow","Pulg","Star","Kren",
                     "Crac","Scaun","Wal","Quer","Ry","Cyn","Rusk","Del",
                     "Lab","Mel","Sep","Lor","Ros","Jar","Daf","Hal","Kol",
                     "In","Ael","Sald","Kuv","Ym","Ca","Keld","Bar","Tarl",
                     "Shot","Pes","Quer","Lor","Geld","Ar","Har","Bae","Vad",
                     "Pas","Ur","Nor","Kir","Var","Mel","Ar","Shy","I","Rald",
                     "Cor","Sar","Kor","Rol","Har","Ash","Dir","Las","Vab",
                     "Ald","Par","Ob","Hor","Chy","Jar","Ryle","Char","Hab",
                     "Sar","Vart","Nist","Obr","Jar","Ge","Yas","Pav","Jes",
                     "Shot","Mar","Hor","Er","Ki","Har","Cal","And",});
    syllables[1] = ({"gur","carn","az","acy","ayn","asc","gary","hen","tan",
                     "arny","alen","carth","gant","rath","cam","art","ron",
                     "arth","arth","carad","ere","geth","aldy","yn","valer",
                     "arne","aller","varn","ar","an","nal","tyne","ar","art",
                     "ont","aur","aver","lyn","as","gar","cuth","arry","or",
                     "quine","astar","mel","aryn","art","war","asty","zane",
                     "arik","ayne","loc","oller","warty","aryne","chean",
                     "quin","tar","dar","reth","ant","an","yne","ax","tuny",
                     "wat","juin","a","gayn","on","an","car","gine","codd",
                     "quent","eas","ew","azer","am","ly","stead","orn","ar",
                     "cath","iera","que","air","la","art","erry","end","om",
                     "ast","et","arty","doth","cath","ert","dy","orn","ont",
                     "tak","ar","art","warne","arn","in","ian","el","ak","il",
                     "ydd","ime","yn","en","in","im","el","ar","ro","is","is",
                     "ro","era","ene","in","ane","iam","ain","ir","un","il",
                     "bin","lin","is","sene","bin","lir","ame","a","fyn","y",
                     "in","yd","ien","ain","yn","ar","er","in","sume","ras",
                     "id","mel","luth","ance","er","yn","an","ar","ayne",
                     "eth","len","ter","rik","er","ro","tin","mel","yn","ris",
                     "lene","ane","as",});
    syllables[2] = ({"ty","carn","ar","acy","er","al","gary","y","ar","arny",
                     "alen","carth","gant","y","ber","art","dal","arth",
                     "arth","an","ere","geth","aldy","yn","valer","arne",
                     "aller","varn","ayne","an","nal","tyne","ayne","art",
                     "ont","ney","aver","lyn","iel","gar","y","arry","or",
                     "quine","astar","er","aryn","art","war","asty","zane",
                     "arik","ayne","an","oller","warty","aryne","chean","ta",
                     "un","tha","reth","ant","el","yne","el","tuny","wat",
                     "juin","dor","gayn","tyn","dar","car","gine","codd",
                     "quent","eas","ew","azer","ont","ly","stead","orn","en",
                     "cath","iera","que","air","la","art","erry","sa","ar",
                     "er","ern","arty","doth","y","ert","dy","orn","ont",
                     "ern","ayn","art","warne","arn","in","ian","el","ak",
                     "il","ydd","ime","yn","en","in","im","el","ar","ro","is",
                     "is","ro","era","ene","in","ane","iam","ain","ir","un",
                     "il","bin","lin","is","sene","bin","lir","ame","a","fyn",
                     "se","in","yd","ien","ain","yn","ar","er","in","sume",
                     "ras","on","mel","luth","ance","er","yn","an","ar",
                     "ayne","eth","nyd","ter","rik","nik","ro","a","mel","yn",
                     "ris","lene","ane","yr",});
    printf("S1 = %O\nS2 = %O\nS3 = %O\n",sizeof(syllables[0]),
                                         sizeof(syllables[1]),
                                         sizeof(syllables[2]));
}

string random_name() {
  string *s;
  int i;
  s = allocate(2+random(2));
  s[0] = syllables[0][random(sizeof(syllables[0]))];
  s[sizeof(s)-1] = syllables[2][random(sizeof(syllables[2]))];
  for (i=1; i<sizeof(s)-1; i++)
    s[i] = syllables[1][random(sizeof(syllables[1]))];
  return implode(s,"");
}

int main() {
    int i;
    printf("Twenty Randomly Generated Names\n"
           "~~~~~~ ~~~~~~~~ ~~~~~~~~~ ~~~~~\n");
    for (i = 1; i <= 20; i++) {
        printf("%2d: %-30s",i,random_name());
        if (!(i&1))
            printf("\n");
    }
    printf("\n");
    return 1;
}
