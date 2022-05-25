#ifndef ANSI_H
#define ANSI_H

#define ESC(X) "["X"m"
#define NORM ESC("0;49;37")
#define GREY_F ESC("1;30")
#define B_BLACK_F ESC("1;30")
#define B_RED_F ESC("1;31")
#define B_GREEN_F ESC("1;32")
#define B_BLUE_F ESC("1;34")
#define B_MAGENTA_F ESC("1;35")
#define B_CYAN_F ESC("1;36")
#define B_BROWN_F ESC("1;33")
#define B_WHITE_F ESC("1;37")
#define WHITE_F ESC("37")
#define BLACK_F ESC("30")
#define RED_F ESC("31")
#define GREEN_F ESC("32")
#define YELLOW_F ESC("1;33")
#define B_YELLOW_F YELLOW_F
#define BLUE_F ESC("34")
#define MAGENTA_F ESC("35")
#define CYAN_F ESC("36")
#define BROWN_F ESC("33")
#define BLACK_B ESC("40")
#define RED_B ESC("41")
#define GREEN_B ESC("42")
#define YELLOW_B ESC("43")
#define BLUE_B ESC("44")
#define MAGENTA_B ESC("45")
#define CYAN_B ESC("46")
#define WHITE_B ESC("47")
#define BOLD ESC("1")
#define FAINT ESC("2")
#define BLINK ESC("5")
#define REVERSE ESC("7")

#define X000 ESC("0;38;5;000")
#define X001 ESC("0;38;5;001")
#define X002 ESC("0;38;5;002")
#define X003 ESC("0;38;5;003")
#define X004 ESC("0;38;5;004")
#define X005 ESC("0;38;5;005")
#define X006 ESC("0;38;5;006")
#define X007 ESC("0;38;5;007")
#define X008 ESC("0;38;5;008")
#define X009 ESC("0;38;5;009")
#define X010 ESC("0;38;5;010")
#define X011 ESC("0;38;5;011")
#define X012 ESC("0;38;5;012")
#define X013 ESC("0;38;5;013")
#define X014 ESC("0;38;5;014")
#define X015 ESC("0;38;5;015")
#define X016 ESC("0;38;5;016")
#define X017 ESC("0;38;5;017")
#define X018 ESC("0;38;5;018")
#define X019 ESC("0;38;5;019")
#define X020 ESC("0;38;5;020")
#define X021 ESC("0;38;5;021")
#define X022 ESC("0;38;5;022")
#define X023 ESC("0;38;5;023")
#define X024 ESC("0;38;5;024")
#define X025 ESC("0;38;5;025")
#define X026 ESC("0;38;5;026")
#define X027 ESC("0;38;5;027")
#define X028 ESC("0;38;5;028")
#define X029 ESC("0;38;5;029")
#define X030 ESC("0;38;5;030")
#define X031 ESC("0;38;5;031")
#define X032 ESC("0;38;5;032")
#define X033 ESC("0;38;5;033")
#define X034 ESC("0;38;5;034")
#define X035 ESC("0;38;5;035")
#define X036 ESC("0;38;5;036")
#define X037 ESC("0;38;5;037")
#define X038 ESC("0;38;5;038")
#define X039 ESC("0;38;5;039")
#define X040 ESC("0;38;5;040")
#define X041 ESC("0;38;5;041")
#define X042 ESC("0;38;5;042")
#define X043 ESC("0;38;5;043")
#define X044 ESC("0;38;5;044")
#define X045 ESC("0;38;5;045")
#define X046 ESC("0;38;5;046")
#define X047 ESC("0;38;5;047")
#define X048 ESC("0;38;5;048")
#define X049 ESC("0;38;5;049")
#define X050 ESC("0;38;5;050")
#define X051 ESC("0;38;5;051")
#define X052 ESC("0;38;5;052")
#define X053 ESC("0;38;5;053")
#define X054 ESC("0;38;5;054")
#define X055 ESC("0;38;5;055")
#define X056 ESC("0;38;5;056")
#define X057 ESC("0;38;5;057")
#define X058 ESC("0;38;5;058")
#define X059 ESC("0;38;5;059")
#define X060 ESC("0;38;5;060")
#define X061 ESC("0;38;5;061")
#define X062 ESC("0;38;5;062")
#define X063 ESC("0;38;5;063")
#define X064 ESC("0;38;5;064")
#define X065 ESC("0;38;5;065")
#define X066 ESC("0;38;5;066")
#define X067 ESC("0;38;5;067")
#define X068 ESC("0;38;5;068")
#define X069 ESC("0;38;5;069")
#define X070 ESC("0;38;5;070")
#define X071 ESC("0;38;5;071")
#define X072 ESC("0;38;5;072")
#define X073 ESC("0;38;5;073")
#define X074 ESC("0;38;5;074")
#define X075 ESC("0;38;5;075")
#define X076 ESC("0;38;5;076")
#define X077 ESC("0;38;5;077")
#define X078 ESC("0;38;5;078")
#define X079 ESC("0;38;5;079")
#define X080 ESC("0;38;5;080")
#define X081 ESC("0;38;5;081")
#define X082 ESC("0;38;5;082")
#define X083 ESC("0;38;5;083")
#define X084 ESC("0;38;5;084")
#define X085 ESC("0;38;5;085")
#define X086 ESC("0;38;5;086")
#define X087 ESC("0;38;5;087")
#define X088 ESC("0;38;5;088")
#define X089 ESC("0;38;5;089")
#define X090 ESC("0;38;5;090")
#define X091 ESC("0;38;5;091")
#define X092 ESC("0;38;5;092")
#define X093 ESC("0;38;5;093")
#define X094 ESC("0;38;5;094")
#define X095 ESC("0;38;5;095")
#define X096 ESC("0;38;5;096")
#define X097 ESC("0;38;5;097")
#define X098 ESC("0;38;5;098")
#define X099 ESC("0;38;5;099")
#define X100 ESC("0;38;5;100")
#define X101 ESC("0;38;5;101")
#define X102 ESC("0;38;5;102")
#define X103 ESC("0;38;5;103")
#define X104 ESC("0;38;5;104")
#define X105 ESC("0;38;5;105")
#define X106 ESC("0;38;5;106")
#define X107 ESC("0;38;5;107")
#define X108 ESC("0;38;5;108")
#define X109 ESC("0;38;5;109")
#define X110 ESC("0;38;5;110")
#define X111 ESC("0;38;5;111")
#define X112 ESC("0;38;5;112")
#define X113 ESC("0;38;5;113")
#define X114 ESC("0;38;5;114")
#define X115 ESC("0;38;5;115")
#define X116 ESC("0;38;5;116")
#define X117 ESC("0;38;5;117")
#define X118 ESC("0;38;5;118")
#define X119 ESC("0;38;5;119")
#define X120 ESC("0;38;5;120")
#define X121 ESC("0;38;5;121")
#define X122 ESC("0;38;5;122")
#define X123 ESC("0;38;5;123")
#define X124 ESC("0;38;5;124")
#define X125 ESC("0;38;5;125")
#define X126 ESC("0;38;5;126")
#define X127 ESC("0;38;5;127")
#define X128 ESC("0;38;5;128")
#define X129 ESC("0;38;5;129")
#define X130 ESC("0;38;5;130")
#define X131 ESC("0;38;5;131")
#define X132 ESC("0;38;5;132")
#define X133 ESC("0;38;5;133")
#define X134 ESC("0;38;5;134")
#define X135 ESC("0;38;5;135")
#define X136 ESC("0;38;5;136")
#define X137 ESC("0;38;5;137")
#define X138 ESC("0;38;5;138")
#define X139 ESC("0;38;5;139")
#define X140 ESC("0;38;5;140")
#define X141 ESC("0;38;5;141")
#define X142 ESC("0;38;5;142")
#define X143 ESC("0;38;5;143")
#define X144 ESC("0;38;5;144")
#define X145 ESC("0;38;5;145")
#define X146 ESC("0;38;5;146")
#define X147 ESC("0;38;5;147")
#define X148 ESC("0;38;5;148")
#define X149 ESC("0;38;5;149")
#define X150 ESC("0;38;5;150")
#define X151 ESC("0;38;5;151")
#define X152 ESC("0;38;5;152")
#define X153 ESC("0;38;5;153")
#define X154 ESC("0;38;5;154")
#define X155 ESC("0;38;5;155")
#define X156 ESC("0;38;5;156")
#define X157 ESC("0;38;5;157")
#define X158 ESC("0;38;5;158")
#define X159 ESC("0;38;5;159")
#define X160 ESC("0;38;5;160")
#define X161 ESC("0;38;5;161")
#define X162 ESC("0;38;5;162")
#define X163 ESC("0;38;5;163")
#define X164 ESC("0;38;5;164")
#define X165 ESC("0;38;5;165")
#define X166 ESC("0;38;5;166")
#define X167 ESC("0;38;5;167")
#define X168 ESC("0;38;5;168")
#define X169 ESC("0;38;5;169")
#define X170 ESC("0;38;5;170")
#define X171 ESC("0;38;5;171")
#define X172 ESC("0;38;5;172")
#define X173 ESC("0;38;5;173")
#define X174 ESC("0;38;5;174")
#define X175 ESC("0;38;5;175")
#define X176 ESC("0;38;5;176")
#define X177 ESC("0;38;5;177")
#define X178 ESC("0;38;5;178")
#define X179 ESC("0;38;5;179")
#define X180 ESC("0;38;5;180")
#define X181 ESC("0;38;5;181")
#define X182 ESC("0;38;5;182")
#define X183 ESC("0;38;5;183")
#define X184 ESC("0;38;5;184")
#define X185 ESC("0;38;5;185")
#define X186 ESC("0;38;5;186")
#define X187 ESC("0;38;5;187")
#define X188 ESC("0;38;5;188")
#define X189 ESC("0;38;5;189")
#define X190 ESC("0;38;5;190")
#define X191 ESC("0;38;5;191")
#define X192 ESC("0;38;5;192")
#define X193 ESC("0;38;5;193")
#define X194 ESC("0;38;5;194")
#define X195 ESC("0;38;5;195")
#define X196 ESC("0;38;5;196")
#define X197 ESC("0;38;5;197")
#define X198 ESC("0;38;5;198")
#define X199 ESC("0;38;5;199")
#define X200 ESC("0;38;5;200")
#define X201 ESC("0;38;5;201")
#define X202 ESC("0;38;5;202")
#define X203 ESC("0;38;5;203")
#define X204 ESC("0;38;5;204")
#define X205 ESC("0;38;5;205")
#define X206 ESC("0;38;5;206")
#define X207 ESC("0;38;5;207")
#define X208 ESC("0;38;5;208")
#define X209 ESC("0;38;5;209")
#define X210 ESC("0;38;5;210")
#define X211 ESC("0;38;5;211")
#define X212 ESC("0;38;5;212")
#define X213 ESC("0;38;5;213")
#define X214 ESC("0;38;5;214")
#define X215 ESC("0;38;5;215")
#define X216 ESC("0;38;5;216")
#define X217 ESC("0;38;5;217")
#define X218 ESC("0;38;5;218")
#define X219 ESC("0;38;5;219")
#define X220 ESC("0;38;5;220")
#define X221 ESC("0;38;5;221")
#define X222 ESC("0;38;5;222")
#define X223 ESC("0;38;5;223")
#define X224 ESC("0;38;5;224")
#define X225 ESC("0;38;5;225")
#define X226 ESC("0;38;5;226")
#define X227 ESC("0;38;5;227")
#define X228 ESC("0;38;5;228")
#define X229 ESC("0;38;5;229")
#define X230 ESC("0;38;5;230")
#define X231 ESC("0;38;5;231")
#define X232 ESC("0;38;5;232")
#define X233 ESC("0;38;5;233")
#define X234 ESC("0;38;5;234")
#define X235 ESC("0;38;5;235")
#define X236 ESC("0;38;5;236")
#define X237 ESC("0;38;5;237")
#define X238 ESC("0;38;5;238")
#define X239 ESC("0;38;5;239")
#define X240 ESC("0;38;5;240")
#define X241 ESC("0;38;5;241")
#define X242 ESC("0;38;5;242")
#define X243 ESC("0;38;5;243")
#define X244 ESC("0;38;5;244")
#define X245 ESC("0;38;5;245")
#define X246 ESC("0;38;5;246")
#define X247 ESC("0;38;5;247")
#define X248 ESC("0;38;5;248")
#define X249 ESC("0;38;5;249")
#define X250 ESC("0;38;5;250")
#define X251 ESC("0;38;5;251")
#define X252 ESC("0;38;5;252")
#define X253 ESC("0;38;5;253")
#define X254 ESC("0;38;5;254")
#define X255 ESC("0;38;5;255")

#endif