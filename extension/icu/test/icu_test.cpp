#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "icu-extension.hpp"
#include "graindb.hpp"
#include "test_helpers.hpp"

using namespace graindb;

TEST_CASE("Test basic ICU extension usage", "[icu]") {
	GrainDB db;
	db.LoadExtension<ICUExtension>();
	Connection con(db);
	unique_ptr<QueryResult> result;

	REQUIRE_NO_FAIL(con.Query("CREATE TABLE strings(s VARCHAR)"));
	REQUIRE_NO_FAIL(con.Query("INSERT INTO strings VALUES ('Gabel'), ('Göbel'), ('Goethe'), ('Goldmann'), ('Göthe'), ('Götz')"));

	// ordering
	result = con.Query("SELECT * FROM strings ORDER BY s COLLATE de");
	REQUIRE(CHECK_COLUMN(result, 0, {"Gabel", "Göbel", "Goethe", "Goldmann", "Göthe", "Götz"}));

	// range filter
	result = con.Query("SELECT * FROM strings WHERE 'Goethe' > s COLLATE de ORDER BY 1");
	REQUIRE(CHECK_COLUMN(result, 0, {"Gabel", "Göbel"}));
	// default binary collation, Göbel is not smaller than Gabel in UTF8 encoding
	result = con.Query("SELECT * FROM strings WHERE 'Goethe' > s ORDER BY 1");
	REQUIRE(CHECK_COLUMN(result, 0, {"Gabel"}));

	// we can also combine this collation with NOCASE
	result = con.Query("SELECT * FROM strings WHERE 'goethe' > s COLLATE de.NOCASE ORDER BY 1");
	REQUIRE(CHECK_COLUMN(result, 0, {"Gabel", "Göbel"}));
	result = con.Query("SELECT * FROM strings WHERE 'goethe' > s COLLATE NOCASE.de ORDER BY 1");
	REQUIRE(CHECK_COLUMN(result, 0, {"Gabel", "Göbel"}));
	// but not with NOACCENT
	REQUIRE_FAIL(con.Query("SELECT * FROM strings WHERE 'goethe' > s COLLATE NOACCENT.de ORDER BY 1"));

	// japanese collation
	REQUIRE_NO_FAIL(con.Query("DELETE FROM strings"));
	REQUIRE_NO_FAIL(con.Query("INSERT INTO strings VALUES ('賃貸人側連絡先 (Lessor side contact)'), ('賃借人側連絡先 (Lessee side contact)'), ('解約連絡先 (Termination contacts)'), ('更新連絡先 (Update contact)')"));

	result = con.Query("SELECT * FROM strings ORDER BY s");
	REQUIRE(CHECK_COLUMN(result, 0, {"更新連絡先 (Update contact)", "解約連絡先 (Termination contacts)", "賃借人側連絡先 (Lessee side contact)", "賃貸人側連絡先 (Lessor side contact)"}));
	result = con.Query("SELECT * FROM strings ORDER BY s COLLATE ja.NOCASE");
	REQUIRE(CHECK_COLUMN(result, 0, {"解約連絡先 (Termination contacts)", "更新連絡先 (Update contact)", "賃借人側連絡先 (Lessee side contact)", "賃貸人側連絡先 (Lessor side contact)"}));
}

static void TestCollation(Connection &con, string collation, vector<string> &characters) {
	unique_ptr<QueryResult> result;
	vector<Value> expected_results;
	vector<idx_t> shuffle_index;
	for(idx_t i = 0; i < characters.size(); i++) {
		expected_results.push_back(Value(characters[i]));
		shuffle_index.push_back(i);
	}
	std::random_shuffle(shuffle_index.begin(), shuffle_index.end());

	REQUIRE_NO_FAIL(con.Query("BEGIN TRANSACTION"));
	REQUIRE_NO_FAIL(con.Query("CREATE TABLE strings(s VARCHAR COLLATE \"" + collation + "\")"));
	for(auto idx : shuffle_index) {
		REQUIRE_NO_FAIL(con.Query("INSERT INTO strings VALUES (?)", Value(characters[idx])));
	}
	result = con.Query("SELECT s FROM strings ORDER BY s");
	if (!CHECK_COLUMN(result, 0, expected_results)) {
		printf("ERROR in collation! %s\n", collation.c_str());
		REQUIRE(false);
	}
	REQUIRE_NO_FAIL(con.Query("ROLLBACK"));
}

// https://unicode.org/cldr/charts/28/collation/index.html
TEST_CASE("Test various collations", "[icu]") {
	GrainDB db;
	db.LoadExtension<ICUExtension>();
	Connection con(db);

	{
		string collation = "af";
		vector<string> characters {"a", "A", "á", "Á", "â", "Â", "b", "B", "c", "C", "d", "D", "e", "E", "é", "É", "è", "È", "ê", "Ê", "ë", "Ë", "f", "F", "g", "G", "h", "H", "i", "I", "î", "Î", "ï", "Ï", "j", "J", "k", "K", "l", "L", "m", "M", "n", "N", "ŉ", "o", "O", "ô", "Ô", "ö", "Ö", "p", "P", "q", "Q", "r", "R", "s", "S", "t", "T", "u", "U", "û", "Û", "v", "V", "w", "W", "x", "X", "y", "Y", "z", "Z"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "sq";
		vector<string> characters {"a", "A", "b", "B", "c", "C", "ç", "Ç", "ḉ", "Ḉ", "d", "D", "dh", "Dh", "DH", "dĥ", "Dĥ", "DĤ", "dȟ", "Dȟ", "DȞ", "dḧ", "Dḧ", "DḦ", "dḣ", "Dḣ", "DḢ", "dḩ", "Dḩ", "DḨ", "dḥ", "Dḥ", "DḤ", "dḫ", "Dḫ", "DḪ", "dẖ", "Dẖ", "e", "E", "ë", "Ë", "ȩ̈", "Ȩ̈", "ę̈", "Ę̈", "ẹ̈", "Ẹ̈", "ḙ̈", "Ḙ̈", "ḛ̈", "Ḛ̈", "f", "F", "g", "G", "gj", "Gj", "GJ", "gĵ", "Gĵ", "GĴ", "gǰ", "Gǰ", "h", "H", "i", "I", "j", "J", "k", "K", "l", "L", "ll", "Ll", "LL", "lĺ", "Lĺ", "LĹ", "lľ", "Lľ", "LĽ", "lļ", "Lļ", "LĻ", "lḷ", "Lḷ", "LḶ", "lḹ", "Lḹ", "LḸ", "lḽ", "Lḽ", "LḼ", "lḻ", "Lḻ", "LḺ", "m", "M", "n", "N", "nj", "Nj", "NJ", "nĵ", "Nĵ", "NĴ", "nǰ", "Nǰ", "o", "O", "p", "P", "q", "Q", "r", "R", "rr", "Rr", "RR", "rŕ", "Rŕ", "RŔ", "rř", "Rř", "RŘ", "rṙ", "Rṙ", "RṘ", "rŗ", "Rŗ", "RŖ", "rȑ", "Rȑ", "RȐ", "rȓ", "Rȓ", "RȒ", "rṛ", "Rṛ", "RṚ", "rṝ", "Rṝ", "RṜ", "rṟ", "Rṟ", "RṞ", "s", "S", "sh", "Sh", "SH", "sĥ", "Sĥ", "SĤ", "sȟ", "Sȟ", "SȞ", "sḧ", "Sḧ", "SḦ", "sḣ", "Sḣ", "SḢ", "sḩ", "Sḩ", "SḨ", "sḥ", "Sḥ", "SḤ", "sḫ", "Sḫ", "SḪ", "sẖ", "Sẖ", "t", "T", "th", "Th", "TH", "tĥ", "Tĥ", "TĤ", "tȟ", "Tȟ", "TȞ", "tḧ", "Tḧ", "TḦ", "tḣ", "Tḣ", "TḢ", "tḩ", "Tḩ", "TḨ", "tḥ", "Tḥ", "TḤ", "tḫ", "Tḫ", "TḪ", "tẖ", "Tẖ", "u", "U", "v", "V", "x", "X", "xh", "Xh", "XH", "xĥ", "Xĥ", "XĤ", "xȟ", "Xȟ", "XȞ", "xḧ", "Xḧ", "XḦ", "xḣ", "Xḣ", "XḢ", "xḩ", "Xḩ", "XḨ", "xḥ", "Xḥ", "XḤ", "xḫ", "Xḫ", "XḪ", "xẖ", "Xẖ", "y", "Y", "z", "Z", "zh", "Zh", "ZH", "zĥ", "Zĥ", "ZĤ", "zȟ", "Zȟ", "ZȞ", "zḧ", "Zḧ", "ZḦ", "zḣ", "Zḣ", "ZḢ", "zḩ", "Zḩ", "ZḨ", "zḥ", "Zḥ", "ZḤ", "zḫ", "Zḫ", "ZḪ", "zẖ", "Zẖ"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "am";
		vector<string> characters {"ሀ", "ሁ", "ሂ", "ሃ", "ሄ", "ህ", "ሆ", "ለ", "ሉ", "ሊ", "ላ", "ሌ", "ል", "ሎ", "ሏ", "ሐ", "ሑ", "ሒ", "ሓ", "ሔ", "ሕ", "ሖ", "ሗ", "መ", "ሙ", "ሚ", "ማ", "ሜ", "ም", "ሞ", "ሟ", "ሠ", "ሡ", "ሢ", "ሣ", "ሤ", "ሥ", "ሦ", "ሧ", "ረ", "ሩ", "ሪ", "ራ", "ሬ", "ር", "ሮ", "ሯ", "ሰ", "ሱ", "ሲ", "ሳ", "ሴ", "ስ", "ሶ", "ሷ", "ሸ", "ሹ", "ሺ", "ሻ", "ሼ", "ሽ", "ሾ", "ሿ", "ቀ", "ቁ", "ቂ", "ቃ", "ቄ", "ቅ", "ቆ", "ቈ", "ቊ", "ቋ", "ቌ", "ቍ", "በ", "ቡ", "ቢ", "ባ", "ቤ", "ብ", "ቦ", "ቧ", "ቨ", "ቩ", "ቪ", "ቫ", "ቬ", "ቭ", "ቮ", "ቯ", "ተ", "ቱ", "ቲ", "ታ", "ቴ", "ት", "ቶ", "ቷ", "ቸ", "ቹ", "ቺ", "ቻ", "ቼ", "ች", "ቾ", "ቿ", "ኀ", "ኁ", "ኂ", "ኃ", "ኄ", "ኅ", "ኆ", "ኈ", "ኊ", "ኋ", "ኌ", "ኍ", "ነ", "ኑ", "ኒ", "ና", "ኔ", "ን", "ኖ", "ኗ", "ኘ", "ኙ", "ኚ", "ኛ", "ኜ", "ኝ", "ኞ", "ኟ", "አ", "ኡ", "ኢ", "ኣ", "ኤ", "እ", "ኦ", "ኧ", "ከ", "ኩ", "ኪ", "ካ", "ኬ", "ክ", "ኮ", "ኰ", "ኲ", "ኳ", "ኴ", "ኵ", "ኸ", "ኹ", "ኺ", "ኻ", "ኼ", "ኽ", "ኾ", "ወ", "ዉ", "ዊ", "ዋ", "ዌ", "ው", "ዎ", "ዐ", "ዑ", "ዒ", "ዓ", "ዔ", "ዕ", "ዖ", "ዘ", "ዙ", "ዚ", "ዛ", "ዜ", "ዝ", "ዞ", "ዟ", "ዠ", "ዡ", "ዢ", "ዣ", "ዤ", "ዥ", "ዦ", "ዧ", "የ", "ዩ", "ዪ", "ያ", "ዬ", "ይ", "ዮ", "ደ", "ዱ", "ዲ", "ዳ", "ዴ", "ድ", "ዶ", "ዷ", "ጀ", "ጁ", "ጂ", "ጃ", "ጄ", "ጅ", "ጆ", "ጇ", "ገ", "ጉ", "ጊ", "ጋ", "ጌ", "ግ", "ጎ", "ጐ", "ጒ", "ጓ", "ጔ", "ጕ", "ጠ", "ጡ", "ጢ", "ጣ", "ጤ", "ጥ", "ጦ", "ጧ", "ጨ", "ጩ", "ጪ", "ጫ", "ጬ", "ጭ", "ጮ", "ጯ", "ጰ", "ጱ", "ጲ", "ጳ", "ጴ", "ጵ", "ጶ", "ጷ", "ጸ", "ጹ", "ጺ", "ጻ", "ጼ", "ጽ", "ጾ", "ጿ", "ፀ", "ፁ", "ፂ", "ፃ", "ፄ", "ፅ", "ፆ", "ፈ", "ፉ", "ፊ", "ፋ", "ፌ", "ፍ", "ፎ", "ፏ", "ፐ", "ፑ", "ፒ", "ፓ", "ፔ", "ፕ", "ፖ", "ፗ"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "ar";
		vector<string> characters {"‫ً", "ﹰ", "ﹱ", "ࣰ", "ࣧ", "ٌ", "ﹲ", "ﱞ", "ࣱ", "ࣨ", "ٍ", "ﹴ", "ﱟ", "ࣲ", "ࣩ", "َ", "ﹶ", "ﹷ", "ﱠ", "ﳲ", "ࣤ", "ࣴ", "ࣵ", "ُ", "ﹸ", "ﹹ", "ﱡ", "ﳳ", "ࣥ", "ࣾ", "ِ", "ﹺ", "ﹻ", "ﱢ", "ﳴ", "ࣦ", "ࣶ", "ّ", "ﹼ", "ﹽ", "ﱣ", "ْ", "ﹾ", "ﹿ", "ٓ", "۟", "ۡ", "ٟ", "ٖ", "ٗ", "٘", "ࣿ", "ٙ", "ٚ", "ٛ", "ٜ", "ٝ", "ٞ", "ࣷ", "ࣸ", "ࣽ", "ࣻ", "ࣼ", "ࣹ", "ࣺ", "ٰ", "ء", "ﺀ", "أ", "ﺃ", "ﺄ", "ؤ", "ﺅ", "ﺆ", "إ", "ﺇ", "ﺈ", "ئ", "ﺉ", "ﺊ", "ﺋ", "ﺌ", "ىٕ", "ا", "ﺍ", "ﺎ", "ﴼ", "ﴽ", "آ", "ﺁ", "ﺂ", "ﯪ", "ﯫ", "ﰀ", "ﲗ", "ﰁ", "ﲘ", "ﲙ", "ﱤ", "ﱥ", "ﷳ", "ﷲ", "ﰂ", "ﱦ", "ﲚ", "ﳟ", "ﱧ", "ﲛ", "ﳠ", "ﯬ", "ﯭ", "ﯮ", "ﯯ", "ﯲ", "ﯳ", "ﯰ", "ﯱ", "ﯴ", "ﯵ", "ﯹ", "ﯺ", "ﯻ", "ﰃ", "ﱨ", "ﰄ", "ﱩ", "ﯶ", "ﯷ", "ﯸ", "ﭐ", "ﭑ", "ب", "ﺏ", "ﺐ", "ﺑ", "ﺒ", "ﰅ", "ﲜ", "ﰆ", "ﲝ", "ﷂ", "ﰇ", "ﲞ", "ﶞ", "ﱪ", "ﱫ", "﷽", "ﰈ", "ﱬ", "ﲟ", "ﳡ", "ﱭ", "ﲠ", "ﳢ", "ﰉ", "ﱮ", "ﰊ", "ﱯ", "ﭒ", "ﭓ", "ﭔ", "ﭕ", "ﭖ", "ﭗ", "ﭘ", "ﭙ", "ﭚ", "ﭛ", "ﭜ", "ﭝ", "ة", "ﺓ", "ﺔ", "ت", "ﺕ", "ﺖ", "ﺗ", "ﺘ", "ﰋ", "ﲡ", "ﵐ", "ﶠ", "ﶟ", "ﰌ", "ﲢ", "ﵑ", "ﵒ", "ﵓ", "ﰍ", "ﲣ", "ﵔ", "ﶢ", "ﶡ", "ﱰ", "ﱱ", "ﰎ", "ﱲ", "ﲤ", "ﳣ", "ﵕ", "ﵖ", "ﵗ", "ﶤ", "ﶣ", "ﱳ", "ﲥ", "ﳤ", "ﰏ", "ﱴ", "ﰐ", "ﱵ", "ث", "ﺙ", "ﺚ", "ﺛ", "ﺜ", "ﰑ", "ﱶ", "ﱷ", "ﰒ", "ﱸ", "ﲦ", "ﳥ", "ﱹ", "ﳦ", "ﰓ", "ﱺ", "ﰔ", "ﱻ", "ﭦ", "ﭧ", "ﭨ", "ﭩ", "ﭞ", "ﭟ", "ﭠ", "ﭡ", "ﭢ", "ﭣ", "ﭤ", "ﭥ", "ج", "ﺝ", "ﺞ", "ﺟ", "ﺠ", "ﰕ", "ﲧ", "ﶦ", "ﶾ", "ﷻ", "ﰖ", "ﲨ", "ﵘ", "ﵙ", "ﶧ", "ﶥ", "ﴁ", "ﴝ", "ﴂ", "ﴞ", "ﭶ", "ﭷ", "ﭸ", "ﭹ", "ﭲ", "ﭳ", "ﭴ", "ﭵ", "ﭺ", "ﭻ", "ﭼ", "ﭽ", "ﭾ", "ﭿ", "ﮀ", "ﮁ", "ح", "ﺡ", "ﺢ", "ﺣ", "ﺤ", "ﰗ", "ﲩ", "ﶿ", "ﰘ", "ﲪ", "ﵛ", "ﵚ", "ﳿ", "ﴛ", "ﴀ", "ﴜ", "خ", "ﺥ", "ﺦ", "ﺧ", "ﺨ", "ﰙ", "ﲫ", "ﰚ", "ﰛ", "ﲬ", "ﴃ", "ﴟ", "ﴄ", "ﴠ", "د", "ﺩ", "ﺪ", "ذ", "ﺫ", "ﺬ", "ﱛ", "ﮈ", "ﮉ", "ﮄ", "ﮅ", "ﮂ", "ﮃ", "ﮆ", "ﮇ", "ر", "ﺭ", "ﺮ", "ﱜ", "ﷶ", "﷼", "ز", "ﺯ", "ﺰ", "ﮌ", "ﮍ", "ﮊ", "ﮋ", "س", "ﺱ", "ﺲ", "ﺳ", "ﺴ", "ﰜ", "ﲭ", "ﴴ", "ﵝ", "ﵞ", "ﰝ", "ﲮ", "ﴵ", "ﵜ", "ﰞ", "ﲯ", "ﴶ", "ﶨ", "ﷆ", "ﴎ", "ﴪ", "ﰟ", "ﲰ", "ﳧ", "ﵡ", "ﵟ", "ﵠ", "ﵢ", "ﵣ", "ﳨ", "ﴱ", "ﳻ", "ﴗ", "ﳼ", "ﴘ", "ش", "ﺵ", "ﺶ", "ﺷ", "ﺸ", "ﴉ", "ﴥ", "ﴭ", "ﴷ", "ﵩ", "ﴊ", "ﴦ", "ﴮ", "ﴸ", "ﵧ", "ﵨ", "ﶪ", "ﴋ", "ﴧ", "ﴯ", "ﴹ", "ﴍ", "ﴩ", "ﳩ", "ﴌ", "ﴨ", "ﴰ", "ﵪ", "ﵫ", "ﵬ", "ﵭ", "ﳪ", "ﴲ", "ﳽ", "ﴙ", "ﳾ", "ﴚ", "ص", "ﺹ", "ﺺ", "ﺻ", "ﺼ", "ﰠ", "ﲱ", "ﵤ", "ﵥ", "ﶩ", "ﲲ", "ﴏ", "ﴫ", "ﷵ", "ﷹ", "ﷺ", "ﷰ", "ﰡ", "ﲳ", "ﵦ", "ﷅ", "ﴅ", "ﴡ", "ﴆ", "ﴢ", "ض", "ﺽ", "ﺾ", "ﺿ", "ﻀ", "ﰢ", "ﲴ", "ﰣ", "ﲵ", "ﵮ", "ﶫ", "ﰤ", "ﲶ", "ﵯ", "ﵰ", "ﴐ", "ﴬ", "ﰥ", "ﲷ", "ﴇ", "ﴣ", "ﴈ", "ﴤ", "ط", "ﻁ", "ﻂ", "ﻃ", "ﻄ", "ﰦ", "ﲸ", "ﰧ", "ﴳ", "ﴺ", "ﵱ", "ﵲ", "ﵳ", "ﵴ", "ﳵ", "ﴑ", "ﳶ", "ﴒ", "ظ", "ﻅ", "ﻆ", "ﻇ", "ﻈ", "ﰨ", "ﲹ", "ﴻ", "ع", "ﻉ", "ﻊ", "ﻋ", "ﻌ", "ﰩ", "ﲺ", "ﵵ", "ﷄ", "ﷷ", "ﰪ", "ﲻ", "ﵶ", "ﵷ", "ﵸ", "ﶶ", "ﳷ", "ﴓ", "ﳸ", "ﴔ", "غ", "ﻍ", "ﻎ", "ﻏ", "ﻐ", "ﰫ", "ﲼ", "ﰬ", "ﲽ", "ﵹ", "ﵻ", "ﵺ", "ﳹ", "ﴕ", "ﳺ", "ﴖ", "ف", "ﻑ", "ﻒ", "ﻓ", "ﻔ", "ﰭ", "ﲾ", "ﰮ", "ﲿ", "ﰯ", "ﳀ", "ﵼ", "ﵽ", "ﰰ", "ﳁ", "ﷁ", "ﰱ", "ﱼ", "ﰲ", "ﱽ", "ﭪ", "ﭫ", "ﭬ", "ﭭ", "ﭮ", "ﭯ", "ﭰ", "ﭱ", "ق", "ﻕ", "ﻖ", "ﻗ", "ﻘ", "ﰳ", "ﳂ", "ﷱ", "ﰴ", "ﳃ", "ﵾ", "ﶴ", "ﵿ", "ﶲ", "ﰵ", "ﱾ", "ﰶ", "ﱿ", "ك", "ﻙ", "ﻚ", "ﻛ", "ﻜ", "ﰷ", "ﲀ", "ﰸ", "ﳄ", "ﰹ", "ﳅ", "ﰺ", "ﳆ", "ﰻ", "ﲁ", "ﳇ", "ﳫ", "ﰼ", "ﲂ", "ﳈ", "ﳬ", "ﶻ", "ﷃ", "ﶷ", "ﰽ", "ﲃ", "ﰾ", "ﲄ", "ﮎ", "ﮏ", "ﮐ", "ﮑ", "ﯓ", "ﯔ", "ﯕ", "ﯖ", "ﮒ", "ﮓ", "ﮔ", "ﮕ", "ﮚ", "ﮛ", "ﮜ", "ﮝ", "ﮖ", "ﮗ", "ﮘ", "ﮙ", "ل", "ﻝ", "ﻞ", "ﻟ", "ﻠ", "ﻷ", "ﻸ", "ﻹ", "ﻺ", "ﻻ", "ﻼ", "ﻵ", "ﻶ", "ﰿ", "ﳉ", "ﶃ", "ﶄ", "ﶺ", "ﶼ", "ﶬ", "ﱀ", "ﳊ", "ﶀ", "ﶵ", "ﶂ", "ﶁ", "ﱁ", "ﳋ", "ﶅ", "ﶆ", "ﱂ", "ﲅ", "ﳌ", "ﳭ", "ﶇ", "ﶈ", "ﶭ", "ﳍ", "ﱃ", "ﲆ", "ﱄ", "ﲇ", "م", "ﻡ", "ﻢ", "ﻣ", "ﻤ", "ﲈ", "ﱅ", "ﳎ", "ﶌ", "ﶒ", "ﶍ", "ﷀ", "ﱆ", "ﳏ", "ﶉ", "ﶊ", "ﷴ", "ﶋ", "ﱇ", "ﳐ", "ﶎ", "ﶏ", "ﶹ", "ﱈ", "ﲉ", "ﳑ", "ﶱ", "ﱉ", "ﱊ", "ن", "ﻥ", "ﻦ", "ﻧ", "ﻨ", "ﱋ", "ﳒ", "ﶸ", "ﶽ", "ﶗ", "ﶘ", "ﶙ", "ﷇ", "ﱌ", "ﳓ", "ﶕ", "ﶖ", "ﶳ", "ﱍ", "ﳔ", "ﲊ", "ﲋ", "ﱎ", "ﲌ", "ﳕ", "ﳮ", "ﶛ", "ﶚ", "ﲍ", "ﳖ", "ﳯ", "ﱏ", "ﲎ", "ﱐ", "ﲏ", "ﮞ", "ﮟ", "ﮠ", "ﮡ", "ﮢ", "ﮣ", "ه", "ﻩ", "ﻪ", "ﻫ", "ﻬ", "ﳙ", "ﱑ", "ﳗ", "ﱒ", "ﳘ", "ﶓ", "ﶔ", "ﱓ", "ﱔ", "ﮪ", "ﮫ", "ﮬ", "ﮭ", "ﮦ", "ﮧ", "ﮨ", "ﮩ", "ﮤ", "ﮥ", "و", "ﻭ", "ﻮ", "ﷸ", "ﯠ", "ﯡ", "ﯙ", "ﯚ", "ﯗ", "ﯘ", "ﯝ", "ﯛ", "ﯜ", "ﯢ", "ﯣ", "ﯞ", "ﯟ", "ی", "ﯼ", "ﯽ", "ﯾ", "ﯿ", "ى", "ﯨ", "ﯩ", "ﻯ", "ﻰ", "ﱝ", "ﲐ", "ي", "ﻱ", "ﻲ", "ﻳ", "ﻴ", "ﱕ", "ﳚ", "ﶯ", "ﱖ", "ﳛ", "ﶮ", "ﱗ", "ﳜ", "ﲑ", "ﲒ", "ﱘ", "ﲓ", "ﳝ", "ﳰ", "ﶜ", "ﶝ", "ﶰ", "ﲔ", "ﳞ", "ﳱ", "ﱙ", "ﲕ", "ﱚ", "ﲖ", "ﯤ", "ﯥ", "ﯦ", "ﯧ", "ﮮ", "ﮯ", "ﮰ", "ﮱ‬"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "hy";
		vector<string> characters {"ա", "Ա", "բ", "Բ", "գ", "Գ", "դ", "Դ", "ե", "Ե", "եւ", "զ", "Զ", "է", "Է", "ը", "Ը", "թ", "Թ", "ժ", "Ժ", "ի", "Ի", "լ", "Լ", "խ", "Խ", "ծ", "Ծ", "կ", "Կ", "հ", "Հ", "ձ", "Ձ", "ղ", "Ղ", "ճ", "Ճ", "մ", "Մ", "յ", "Յ", "ն", "Ն", "շ", "Շ", "ո", "Ո", "չ", "Չ", "պ", "Պ", "ջ", "Ջ", "ռ", "Ռ", "ս", "Ս", "վ", "Վ", "տ", "Տ", "ր", "Ր", "ց", "Ց", "ւ", "Ւ", "փ", "Փ", "ք", "Ք", "և", "Եւ", "օ", "Օ", "ֆ", "Ֆ"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "as";
		vector<string> characters {"়", "অ", "আ", "ই", "ঈ", "উ", "ঊ", "ঋ", "এ", "ঐ", "ও", "ঔ", "ং", "ঁ", "ঃ", "ক", "খ", "গ", "ঘ", "ঙ", "চ", "ছ", "জ", "ঝ", "ঞ", "ট", "ঠ", "ড", "ড়", "ঢ", "ঢ়", "ণ", "ত্‍", "ৎ", "ত", "থ", "দ", "ধ", "ন", "প", "ফ", "ব", "ভ", "ম", "য", "য়", "ৰ", "ল", "ৱ", "শ", "ষ", "স", "হ", "ক্ষ", "া", "ি", "ী", "ু", "ূ", "ৃ", "ে", "ৈ", "ো", "ৌ", "্"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "az";
		vector<string> characters {"a", "A", "b", "B", "c", "C", "ç", "Ç", "ḉ", "Ḉ", "d", "D", "e", "E", "ə", "Ə", "f", "F", "g", "G", "ğ", "Ğ", "ģ̆", "Ģ̆", "h", "H", "x", "X", "ẍ", "Ẍ", "ẋ", "Ẋ", "ı", "I", "Í", "Ì", "Ĭ", "Î", "Ǐ", "Ï", "Ḯ", "Ĩ", "Į", "Ī", "Ỉ", "Ȉ", "Ȋ", "Ị", "Ḭ", "i", "İ", "Į̇", "Ị̇", "Ḭ̇", "j", "J", "k", "K", "q", "Q", "l", "L", "m", "M", "n", "N", "o", "O", "ö", "Ö", "ǫ̈", "Ǫ̈", "ȫ", "Ȫ", "ơ̈", "Ơ̈", "ợ̈", "Ợ̈", "ọ̈", "Ọ̈", "p", "P", "r", "R", "s", "S", "ş", "Ş", "t", "T", "u", "U", "ü", "Ü", "ǘ", "Ǘ", "ǜ", "Ǜ", "ǚ", "Ǚ", "ų̈", "Ų̈", "ǖ", "Ǖ", "ư̈", "Ư̈", "ự̈", "Ự̈", "ụ̈", "Ụ̈", "ṳ̈", "Ṳ̈", "ṷ̈", "Ṷ̈", "ṵ̈", "Ṵ̈", "v", "V", "y", "Y", "z", "Z", "w", "W", "ẃ", "Ẃ", "ẁ", "Ẁ", "ŵ", "Ŵ", "ẘ", "ẅ", "Ẅ", "ẇ", "Ẇ", "ẉ", "Ẉ", "ﺎ", "ﺍ", "آ", "ﺂ", "ﺁ", "أ", "ﺄ", "ﺃ", "إ", "ﺈ", "ﺇ", "ﻫ", "ﻬ", "ﻪ", "ﻩ", "ة", "ﺔ", "ﺓ", "ۥ", "ﻮ", "ﻭ", "ؤ", "ﺆ", "ﺅ", "ۦ", "ﻳ", "ﻴ", "ﻲ", "ﻱ", "ئ", "ﺋ", "ﺌ", "ﺊ", "ﺉ", "ى", "ﯨ", "ﯩ", "ﻰ", "ﻯ", "เก", "เข", "เฃ", "เค", "เฅ", "เฆ", "เง", "เจ", "เฉ", "เช", "เซ", "เฌ", "เญ", "เฎ", "เฏ", "เฐ", "เฑ", "เฒ", "เณ", "เด", "เต", "เถ", "เท", "เธ", "เน", "เบ", "เป", "เผ", "เฝ", "เพ", "เฟ", "เภ", "เม", "เย", "เร", "เฤ", "เล", "เฦ", "เว", "เศ", "เษ", "เส", "เห", "เฬ", "เอ", "เฮ", "แก", "แข", "แฃ", "แค", "แฅ", "แฆ", "แง", "แจ", "แฉ", "แช", "แซ", "แฌ", "แญ", "แฎ", "แฏ", "แฐ", "แฑ", "แฒ", "แณ", "แด", "แต", "แถ", "แท", "แธ", "แน", "แบ", "แป", "แผ", "แฝ", "แพ", "แฟ", "แภ", "แม", "แย", "แร", "แฤ", "แล", "แฦ", "แว", "แศ", "แษ", "แส", "แห", "แฬ", "แอ", "แฮ", "โก", "โข", "โฃ", "โค", "โฅ", "โฆ", "โง", "โจ", "โฉ", "โช", "โซ", "โฌ", "โญ", "โฎ", "โฏ", "โฐ", "โฑ", "โฒ", "โณ", "โด", "โต", "โถ", "โท", "โธ", "โน", "โบ", "โป", "โผ", "โฝ", "โพ", "โฟ", "โภ", "โม", "โย", "โร", "โฤ", "โล", "โฦ", "โว", "โศ", "โษ", "โส", "โห", "โฬ", "โอ", "โฮ", "ใก", "ใข", "ใฃ", "ใค", "ใฅ", "ใฆ", "ใง", "ใจ", "ใฉ", "ใช", "ใซ", "ใฌ", "ใญ", "ใฎ", "ใฏ", "ใฐ", "ใฑ"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "be";
		vector<string> characters {"а", "А", "б", "Б", "в", "В", "г", "Г", "д", "Д", "дж", "дз", "е", "Е", "ё", "Ё", "ж", "Ж", "з", "З", "і", "І", "й", "Й", "к", "К", "л", "Л", "м", "М", "н", "Н", "о", "О", "п", "П", "р", "Р", "с", "С", "т", "Т", "у", "У", "ў", "Ў", "ф", "Ф", "х", "Х", "ц", "Ц", "ч", "Ч", "ш", "Ш", "ы", "Ы", "ь", "Ь", "э", "Э", "ю", "Ю", "я", "Я"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "bn";
		vector<string> characters {"়", "৺", "অ", "আ", "ই", "ঈ", "উ", "ঊ", "ঋ", "ৠ", "ঌ", "ৡ", "এ", "ঐ", "ও", "ঔ", "ং", "ঃ", "ঁ", "ক্", "ক়্", "ক", "ক়", "কা", "ক়া", "কি", "ক়ি", "কী", "ক়ী", "কু", "ক়ু", "কূ", "ক়ূ", "কৃ", "ক়ৃ", "কৄ", "ক়ৄ", "কৢ", "ক়ৢ", "কৣ", "ক়ৣ", "কে", "ক়ে", "কৈ", "ক়ৈ", "কো", "ক়ো", "কৌ", "ক়ৌ", "ক্ষ", "খ্", "খ়্", "খ", "খ়", "খা", "খ়া", "খি", "খ়ি", "খী", "খ়ী", "খু", "খ়ু", "খূ", "খ়ূ", "খৃ", "খ়ৃ", "খৄ", "খ়ৄ", "খৢ", "খ়ৢ", "খৣ", "খ়ৣ", "খে", "খ়ে", "খৈ", "খ়ৈ", "খো", "খ়ো", "খৌ", "খ়ৌ", "গ্", "গ়্", "গ", "গ়", "গা", "গ়া", "গি", "গ়ি", "গী", "গ়ী", "গু", "গ়ু", "গূ", "গ়ূ", "গৃ", "গ়ৃ", "গৄ", "গ়ৄ", "গৢ", "গ়ৢ", "গৣ", "গ়ৣ", "গে", "গ়ে", "গৈ", "গ়ৈ", "গো", "গ়ো", "গৌ", "গ়ৌ", "ঘ্", "ঘ", "ঘা", "ঘি", "ঘী", "ঘু", "ঘূ", "ঘৃ", "ঘৄ", "ঘৢ", "ঘৣ", "ঘে", "ঘৈ", "ঘো", "ঘৌ", "ঙ্", "ঙ", "ঙা", "ঙি", "ঙী", "ঙু", "ঙূ", "ঙৃ", "ঙৄ", "ঙৢ", "ঙৣ", "ঙে", "ঙৈ", "ঙো", "ঙৌ", "চ্", "চ", "চা", "চি", "চী", "চু", "চূ", "চৃ", "চৄ", "চৢ", "চৣ", "চে", "চৈ", "চো", "চৌ", "ছ্", "ছ", "ছা", "ছি", "ছী", "ছু", "ছূ", "ছৃ", "ছৄ", "ছৢ", "ছৣ", "ছে", "ছৈ", "ছো", "ছৌ", "জ্", "জ়্", "জ", "জ়", "জা", "জ়া", "জি", "জ়ি", "জী", "জ়ী", "জু", "জ়ু", "জূ", "জ়ূ", "জৃ", "জ়ৃ", "জৄ", "জ়ৄ", "জৢ", "জ়ৢ", "জৣ", "জ়ৣ", "জে", "জ়ে", "জৈ", "জ়ৈ", "জো", "জ়ো", "জৌ", "জ়ৌ", "ঝ্", "ঝ", "ঝা", "ঝি", "ঝী", "ঝু", "ঝূ", "ঝৃ", "ঝৄ", "ঝৢ", "ঝৣ", "ঝে", "ঝৈ", "ঝো", "ঝৌ", "ঞ্", "ঞ", "ঞা", "ঞি", "ঞী", "ঞু", "ঞূ", "ঞৃ", "ঞৄ", "ঞৢ", "ঞৣ", "ঞে", "ঞৈ", "ঞো", "ঞৌ", "ট্", "ট", "টা", "টি", "টী", "টু", "টূ", "টৃ", "টৄ", "টৢ", "টৣ", "টে", "টৈ", "টো", "টৌ", "ঠ্", "ঠ", "ঠা", "ঠি", "ঠী", "ঠু", "ঠূ", "ঠৃ", "ঠৄ", "ঠৢ", "ঠৣ", "ঠে", "ঠৈ", "ঠো", "ঠৌ", "ড্", "ড়্", "ড", "ড়", "ডা", "ড়া", "ডি", "ড়ি", "ডী", "ড়ী", "ডু", "ড়ু", "ডূ", "ড়ূ", "ডৃ", "ড়ৃ", "ডৄ", "ড়ৄ", "ডৢ", "ড়ৢ", "ডৣ", "ড়ৣ", "ডে", "ড়ে", "ডৈ", "ড়ৈ", "ডো", "ড়ো", "ডৌ", "ড়ৌ", "ঢ্", "ঢ়্", "ঢ", "ঢ়", "ঢা", "ঢ়া", "ঢি", "ঢ়ি", "ঢী", "ঢ়ী", "ঢু", "ঢ়ু", "ঢূ", "ঢ়ূ", "ঢৃ", "ঢ়ৃ", "ঢৄ", "ঢ়ৄ", "ঢৢ", "ঢ়ৢ", "ঢৣ", "ঢ়ৣ", "ঢে", "ঢ়ে", "ঢৈ", "ঢ়ৈ", "ঢো", "ঢ়ো", "ঢৌ", "ঢ়ৌ", "ণ্", "ণ", "ণা", "ণি", "ণী", "ণু", "ণূ", "ণৃ", "ণৄ", "ণৢ", "ণৣ", "ণে", "ণৈ", "ণো", "ণৌ", "ত্‍", "ৎ", "ত্", "ত", "তা", "তি", "তী", "তু", "তূ", "তৃ", "তৄ", "তৢ", "তৣ", "তে", "তৈ", "তো", "তৌ", "থ্", "থ", "থা", "থি", "থী", "থু", "থূ", "থৃ", "থৄ", "থৢ", "থৣ", "থে", "থৈ", "থো", "থৌ", "দ্", "দ", "দা", "দি", "দী", "দু", "দূ", "দৃ", "দৄ", "দৢ", "দৣ", "দে", "দৈ", "দো", "দৌ", "ধ্", "ধ", "ধা", "ধি", "ধী", "ধু", "ধূ", "ধৃ", "ধৄ", "ধৢ", "ধৣ", "ধে", "ধৈ", "ধো", "ধৌ", "ন্", "ন", "না", "নি", "নী", "নু", "নূ", "নৃ", "নৄ", "নৢ", "নৣ", "নে", "নৈ", "নো", "নৌ", "প্", "প", "পা", "পি", "পী", "পু", "পূ", "পৃ", "পৄ", "পৢ", "পৣ", "পে", "পৈ", "পো", "পৌ", "ফ্", "ফ়্", "ফ", "ফ়", "ফা", "ফ়া", "ফি", "ফ়ি", "ফী", "ফ়ী", "ফু", "ফ়ু", "ফূ", "ফ়ূ", "ফৃ", "ফ়ৃ", "ফৄ", "ফ়ৄ", "ফৢ", "ফ়ৢ", "ফৣ", "ফ়ৣ", "ফে", "ফ়ে", "ফৈ", "ফ়ৈ", "ফো", "ফ়ো", "ফৌ", "ফ়ৌ", "ব্", "ব়্", "ব", "ব়", "বা", "ব়া", "বি", "ব়ি", "বী", "ব়ী", "বু", "ব়ু", "বূ", "ব়ূ", "বৃ", "ব়ৃ", "বৄ", "ব়ৄ", "বৢ", "ব়ৢ", "বৣ", "ব়ৣ", "বে", "ব়ে", "বৈ", "ব়ৈ", "বো", "ব়ো", "বৌ", "ব়ৌ", "ভ্", "ভ", "ভা", "ভি", "ভী", "ভু", "ভূ", "ভৃ", "ভৄ", "ভৢ", "ভৣ", "ভে", "ভৈ", "ভো", "ভৌ", "ম্", "ম", "মা", "মি", "মী", "মু", "মূ", "মৃ", "মৄ", "মৢ", "মৣ", "মে", "মৈ", "মো", "মৌ", "য্", "য়্", "য", "য়", "যা", "য়া", "যি", "য়ি", "যী", "য়ী", "যু", "য়ু", "যূ", "য়ূ", "যৃ", "য়ৃ", "যৄ", "য়ৄ", "যৢ", "য়ৢ", "যৣ", "য়ৣ", "যে", "য়ে", "যৈ", "য়ৈ", "যো", "য়ো", "যৌ", "য়ৌ", "র্", "র", "রা", "রি", "রী", "রু", "রূ", "রৃ", "রৄ", "রৢ", "রৣ", "রে", "রৈ", "রো", "রৌ", "ৰ্", "ৰ", "ৰা", "ৰি", "ৰী", "ৰু", "ৰূ", "ৰৃ", "ৰৄ", "ৰৢ", "ৰৣ", "ৰে", "ৰৈ", "ৰো", "ৰৌ", "ল্", "ল", "লা", "লি", "লী", "লু", "লূ", "লৃ", "লৄ", "লৢ", "লৣ", "লে", "লৈ", "লো", "লৌ", "ৱ্", "ৱ", "ৱা", "ৱি", "ৱী", "ৱু", "ৱূ", "ৱৃ", "ৱৄ", "ৱৢ", "ৱৣ", "ৱে", "ৱৈ", "ৱো", "ৱৌ", "শ্", "শ", "শা", "শি", "শী", "শু", "শূ", "শৃ", "শৄ", "শৢ", "শৣ", "শে", "শৈ", "শো", "শৌ", "ষ্", "ষ", "ষা", "ষি", "ষী", "ষু", "ষূ", "ষৃ", "ষৄ", "ষৢ", "ষৣ", "ষে", "ষৈ", "ষো", "ষৌ", "স্", "স", "সা", "সি", "সী", "সু", "সূ", "সৃ", "সৄ", "সৢ", "সৣ", "সে", "সৈ", "সো", "সৌ", "হ্", "হ", "হা", "হি", "হী", "হু", "হূ", "হৃ", "হৄ", "হৢ", "হৣ", "হে", "হৈ", "হো", "হৌ", "ঽ", "া", "ি", "ী", "ু", "ূ", "ৃ", "ৄ", "ৢ", "ৣ", "ে", "ৈ", "ো", "ৌ", "্", "ৗ"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "bs";
		vector<string> characters {"a", "A", "b", "B", "c", "C", "č", "Č", "ç̌", "Ç̌", "ć", "Ć", "ḉ", "Ḉ", "d", "D", "dž", "ǆ", "Dž", "ǅ", "DŽ", "Ǆ", "đ", "Đ", "e", "E", "f", "F", "g", "G", "h", "H", "i", "I", "j", "J", "k", "K", "l", "L", "lj", "ǉ", "Lj", "ǈ", "LJ", "Ǉ", "lĵ", "Lĵ", "LĴ", "lǰ", "Lǰ", "m", "M", "n", "N", "nj", "ǌ", "Nj", "ǋ", "NJ", "Ǌ", "nĵ", "Nĵ", "NĴ", "nǰ", "Nǰ", "o", "O", "p", "P", "r", "R", "s", "S", "š", "Š", "ṧ", "Ṧ", "ş̌", "Ş̌", "ṣ̌", "Ṣ̌", "ș̌", "Ș̌", "t", "T", "u", "U", "v", "V", "z", "Z", "ž", "Ž", "ẓ̌", "Ẓ̌", "ẕ̌", "Ẕ̌", "ﺎ", "ﺍ", "آ", "ﺂ", "ﺁ", "أ", "ﺄ", "ﺃ", "إ", "ﺈ", "ﺇ", "ﻫ", "ﻬ", "ﻪ", "ﻩ", "ة", "ﺔ", "ﺓ", "ۥ", "ﻮ", "ﻭ", "ؤ", "ﺆ", "ﺅ", "ۦ", "ﻳ", "ﻴ", "ﻲ", "ﻱ", "ئ", "ﺋ", "ﺌ", "ﺊ", "ﺉ", "ى", "ﯨ", "ﯩ", "ﻰ", "ﻯ", "เก", "เข", "เฃ", "เค", "เฅ", "เฆ", "เง", "เจ", "เฉ", "เช", "เซ", "เฌ", "เญ", "เฎ", "เฏ", "เฐ", "เฑ", "เฒ", "เณ", "เด", "เต", "เถ", "เท", "เธ", "เน", "เบ", "เป", "เผ", "เฝ", "เพ", "เฟ", "เภ", "เม", "เย", "เร", "เฤ", "เล", "เฦ", "เว", "เศ", "เษ", "เส", "เห", "เฬ", "เอ", "เฮ", "แก", "แข", "แฃ", "แค", "แฅ", "แฆ", "แง", "แจ", "แฉ", "แช", "แซ", "แฌ", "แญ", "แฎ", "แฏ", "แฐ", "แฑ", "แฒ", "แณ", "แด", "แต", "แถ", "แท", "แธ", "แน", "แบ", "แป", "แผ", "แฝ", "แพ", "แฟ", "แภ", "แม", "แย", "แร", "แฤ", "แล", "แฦ", "แว", "แศ", "แษ", "แส", "แห", "แฬ", "แอ", "แฮ", "โก", "โข", "โฃ", "โค", "โฅ", "โฆ", "โง", "โจ", "โฉ", "โช", "โซ", "โฌ", "โญ", "โฎ", "โฏ", "โฐ", "โฑ", "โฒ", "โณ", "โด", "โต", "โถ", "โท", "โธ", "โน", "โบ", "โป", "โผ", "โฝ", "โพ", "โฟ", "โภ", "โม", "โย", "โร", "โฤ", "โล", "โฦ", "โว", "โศ", "โษ", "โส", "โห", "โฬ", "โอ", "โฮ", "ใก", "ใข", "ใฃ", "ใค", "ใฅ", "ใฆ", "ใง", "ใจ", "ใฉ", "ใช", "ใซ", "ใฌ", "ใญ", "ใฎ", "ใฏ", "ใฐ", "ใฑ", "ใฒ", "ใณ", "ใด", "ใต", "ใถ", "ใท", "ใธ", "ใน", "ใบ", "ใป", "ใผ", "ใฝ", "ใพ", "ใฟ", "ใภ", "ใม", "ใย", "ใร", "ใฤ", "ใล", "ใฦ", "ใว", "ใศ", "ใษ", "ใส", "ให", "ใฬ", "ใอ", "ใฮ", "ไก", "ไข", "ไฃ", "ไค", "ไฅ", "ไฆ", "ไง", "ไจ", "ไฉ", "ไช", "ไซ", "ไฌ", "ไญ", "ไฎ", "ไฏ", "ไฐ", "ไฑ", "ไฒ", "ไณ", "ได", "ไต", "ไถ", "ไท", "ไธ", "ไน", "ไบ", "ไป", "ไผ", "ไฝ", "ไพ", "ไฟ", "ไภ", "ไม", "ไย", "ไร", "ไฤ", "ไล", "ไฦ", "ไว", "ไศ", "ไษ", "ไส", "ไห", "ไฬ", "ไอ", "ไฮ", "ເໞ", "ເກ", "ເຂ", "ເຄ", "ເງ", "ເຈ", "ເສ", "ເຊ", "ເໟ", "ເຍ", "ເດ", "ເຕ", "ເຖ", "ເທ", "ເນ", "ເບ", "ເປ", "ເຜ", "ເຝ", "ເພ", "ເຟ", "ເມ", "ເຢ", "ເຣ", "ເລ", "ເວ", "ເຫ", "ເໜ", "ເໝ", "ເອ", "ເຮ", "ແໞ", "ແກ", "ແຂ", "ແຄ", "ແງ", "ແຈ", "ແສ", "ແຊ", "ແໟ", "ແຍ", "ແດ", "ແຕ", "ແຖ", "ແທ", "ແນ", "ແບ", "ແປ", "ແຜ", "ແຝ", "ແພ", "ແຟ", "ແມ", "ແຢ", "ແຣ", "ແລ", "ແວ", "ແຫ", "ແໜ", "ແໝ", "ແອ", "ແຮ", "ໂໞ", "ໂກ", "ໂຂ", "ໂຄ", "ໂງ", "ໂຈ", "ໂສ", "ໂຊ", "ໂໟ", "ໂຍ", "ໂດ", "ໂຕ", "ໂຖ", "ໂທ", "ໂນ", "ໂບ", "ໂປ", "ໂຜ", "ໂຝ", "ໂພ", "ໂຟ", "ໂມ", "ໂຢ", "ໂຣ", "ໂລ", "ໂວ", "ໂຫ", "ໂໜ", "ໂໝ", "ໂອ", "ໂຮ", "ໃໞ", "ໃກ", "ໃຂ", "ໃຄ", "ໃງ", "ໃຈ", "ໃສ", "ໃຊ", "ໃໟ", "ໃຍ", "ໃດ", "ໃຕ", "ໃຖ", "ໃທ", "ໃນ", "ໃບ", "ໃປ", "ໃຜ", "ໃຝ", "ໃພ", "ໃຟ", "ໃມ", "ໃຢ", "ໃຣ", "ໃລ", "ໃວ", "ໃຫ", "ໃໜ", "ໃໝ", "ໃອ", "ໃຮ", "ໄໞ", "ໄກ", "ໄຂ", "ໄຄ", "ໄງ", "ໄຈ", "ໄສ", "ໄຊ", "ໄໟ", "ໄຍ", "ໄດ", "ໄຕ", "ໄຖ", "ໄທ", "ໄນ", "ໄບ", "ໄປ", "ໄຜ", "ໄຝ", "ໄພ", "ໄຟ", "ໄມ", "ໄຢ", "ໄຣ", "ໄລ", "ໄວ", "ໄຫ", "ໄໜ", "ໄໝ", "ໄອ", "ໄຮ", "ꪵꪀ", "ꪵꪁ", "ꪵꪂ", "ꪵꪃ", "ꪵꪄ", "ꪵꪅ", "ꪵꪆ"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "bg";
		vector<string> characters {"а", "А", "б", "Б", "в", "В", "г", "Г", "д", "Д", "е", "Е", "ж", "Ж", "з", "З", "и", "И", "й", "Й", "к", "К", "л", "Л", "м", "М", "н", "Н", "о", "О", "п", "П", "р", "Р", "с", "С", "т", "Т", "у", "У", "ф", "Ф", "х", "Х", "ц", "Ц", "ч", "Ч", "ш", "Ш", "щ", "Щ", "ъ", "Ъ", "ь", "Ь", "ю", "Ю", "я", "Я"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "my";
		vector<string> characters {"က", "ခ", "ဂ", "ဃ", "င", "စ", "ဆ", "ဇ", "ဈ", "ဉ", "ည", "ဋ", "ဌ", "ဍ", "ဎ", "ဏ", "တ", "ထ", "ထ္မင်း", "ဒ", "ဓ", "န", "၌", "ပ", "ဖ", "ဗ", "ဘ", "မ", "ယ", "ရ", "၍", "လ", "လ္ဘက်", "လက်ျာ", "၎င်း", "ဝ", "သ", "သ္မီ", "ဟ", "ဠ", "အ", "ဣ", "၏", "ဣင္", "ဤ", "ဥ", "ဥင္", "ဦ", "ဧ", "ဨ", "ဧင္", "ဩ", "ဩောင္", "ဩင္", "ဪ", "ဣက္", "ဣက်", "ဥက္", "ဥက်", "ဧက္", "ဧက်", "ဩောက္", "ဩက္", "ဩောက်", "ဩက်", "ဣခ္", "ဣခ်", "ဥခ္", "ဥခ်", "ဧခ္", "ဧခ်", "ဩောခ္", "ဩခ္", "ဩောခ်", "ဩခ်", "ဣဂ္", "ဣဂ်", "ဥဂ္", "ဥဂ်", "ဧဂ္", "ဧဂ်", "ဩောဂ္", "ဩဂ္", "ဩောဂ်", "ဩဂ်", "ဣဃ္", "ဣဃ်", "ဥဃ္", "ဥဃ်", "ဧဃ္", "ဧဃ်", "ဩောဃ္", "ဩဃ္", "ဩောဃ်", "ဩဃ်", "ဣင်", "ဥင်", "ဧင်", "ဩောင်", "ဩင်", "ဣစ္", "ဣစ်", "ဥစ္", "ဥစ်", "ဧစ္", "ဧစ်", "ဩောစ္", "ဩစ္", "ဩောစ်", "ဩစ်", "ဣဆ္", "ဣဆ်", "ဥဆ္", "ဥဆ်", "ဧဆ္", "ဧဆ်", "ဩောဆ္", "ဩဆ္", "ဩောဆ်", "ဩဆ်", "ဣဇ္", "ဣဇ်", "ဥဇ္", "ဥဇ်", "ဧဇ္", "ဧဇ်", "ဩောဇ္", "ဩဇ္", "ဩောဇ်", "ဩဇ်", "ဣဉ္", "ဣဉ်", "ဥဉ္", "ဥဉ်", "ဧဉ္", "ဧဉ်", "ဩောဉ္", "ဩဉ္", "ဩောဉ်", "ဩဉ်", "ဣည္", "ဣည်", "ဥည္", "ဥည်", "ဧည္", "ဧည်", "ဩောည္", "ဩည္", "ဩောည်", "ဩည်", "ဣဋ္", "ဣဋ်", "ဥဋ္", "ဥဋ်", "ဧဋ္", "ဧဋ်", "ဩောဋ္", "ဩဋ္", "ဩောဋ်", "ဩဋ်", "ဣဌ္", "ဣဌ်", "ဥဌ္", "ဥဌ်", "ဧဌ္", "ဧဌ်", "ဩောဌ္", "ဩဌ္", "ဩောဌ်", "ဩဌ်", "ဣဍ္", "ဣဍ်", "ဥဍ္", "ဥဍ်", "ဧဍ္", "ဧဍ်", "ဩောဍ္", "ဩဍ္", "ဩောဍ်", "ဩဍ်", "ဣဎ္", "ဣဎ်", "ဥဎ္", "ဥဎ်", "ဧဎ္", "ဧဎ်", "ဩောဎ္", "ဩဎ္", "ဩောဎ်", "ဩဎ်", "ဣဏ္", "ဣဏ်", "ဥဏ္", "ဥဏ်", "ဧဏ္", "ဧဏ်", "ဩောဏ္", "ဩဏ္", "ဩောဏ်", "ဩဏ်", "ဣတ္", "ဣတ်", "ဥတ္", "ဥတ်", "ဧတ္", "ဧတ်", "ဩောတ္", "ဩတ္", "ဩောတ်", "ဩတ်", "ဣထ္", "ဣထ်", "ဥထ္", "ဥထ်", "ဧထ္", "ဧထ်", "ဩောထ္", "ဩထ္", "ဩောထ်", "ဩထ်", "ဣဒ္", "ဣဒ်", "ဥဒ္", "ဥဒ်", "ဧဒ္", "ဧဒ်", "ဩောဒ္", "ဩဒ္", "ဩောဒ်", "ဩဒ်", "ဣဓ္", "ဣဓ်", "ဥဓ္", "ဥဓ်", "ဧဓ္", "ဧဓ်", "ဩောဓ္", "ဩဓ္", "ဩောဓ်", "ဩဓ်", "ဣန္", "ဣန်", "ဥန္", "ဥန်", "ဧန္", "ဧန်", "ဩောန္", "ဩန္", "ဩောန်", "ဩန်", "ဣပ္", "ဣပ်", "ဥပ္", "ဥပ်", "ဧပ္", "ဧပ်", "ဩောပ္", "ဩပ္", "ဩောပ်", "ဩပ်", "ဣဖ္", "ဣဖ်", "ဥဖ္", "ဥဖ်", "ဧဖ္", "ဧဖ်", "ဩောဖ္", "ဩဖ္", "ဩောဖ်", "ဩဖ်", "ဣဗ္", "ဣဗ်", "ဥဗ္", "ဥဗ်", "ဧဗ္", "ဧဗ်", "ဩောဗ္", "ဩဗ္", "ဩောဗ်", "ဩဗ်", "ဣဘ္", "ဣဘ်", "ဥဘ္", "ဥဘ်", "ဧဘ္", "ဧဘ်", "ဩောဘ္", "ဩဘ္", "ဩောဘ်", "ဩဘ်", "ဣမ္", "ဣမ်", "ဥမ္", "ဥမ်", "ဥုံ", "ဧမ္", "ဧမ်", "ဩောမ္", "ဩမ္", "ဩောမ်", "ဩမ်", "ဣယ္", "ဣယ်", "ဥယ္", "ဥယ်", "ဧယ္", "ဧယ်", "ဩောယ္", "ဩယ္", "ဩောယ်", "ဩယ်", "ဣရ္", "ဣရ်", "ဥရ္", "ဥရ်", "ဧရ္", "ဧရ်", "ဩောရ္", "ဩရ္", "ဩောရ်", "ဩရ်", "ဣလ္", "ဣလ်", "ဥလ္", "ဥလ်", "ဧလ္", "ဧလ်", "ဩောလ္", "ဩလ္", "ဩောလ်", "ဩလ်", "ဣသ္", "ဣသ်", "ဣဿ", "ဥသ္", "ဥသ်", "ဥဿ", "ဧသ္", "ဧသ်", "ဩောသ္", "ဩသ္", "ဩောသ်", "ဩသ်", "ဣဟ္", "ဣဟ်", "ဥဟ္", "ဥဟ်", "ဧဟ္", "ဧဟ်", "ဩောဟ္", "ဩဟ္", "ဩောဟ်", "ဩဟ်", "ာ", "ါ", "ိ", "ီ", "ု", "ူ", "ေ", "ဲ", "ော", "ေါ", "ော်", "ေါ်", "ံ", "ို", "က္", "က်", "ာက္", "ာက်", "ါက္", "ါက်", "ိက္", "ိက်", "ုက္", "ုက်", "ေက္", "ေက်", "ောက္", "ောက်", "ေါက္", "ေါက်", "ောက်ျ", "ိုက္", "ိုက်", "ခ္", "ခ်", "ာခ္", "ာခ်", "ါခ္", "ါခ်", "ိခ္", "ိခ်", "ုခ္", "ုခ်", "ေခ္", "ေခ်", "ောခ္", "ောခ်", "ေါခ္", "ေါခ်", "ိုခ္", "ိုခ်", "ဂ္", "ဂ်", "ာဂ္", "ာဂ်", "ါဂ္", "ါဂ်", "ိဂ္", "ိဂ်", "ုဂ္", "ုဂ်", "ေဂ္", "ေဂ်", "ောဂ္", "ောဂ်", "ေါဂ္", "ေါဂ်", "ိုဂ္", "ိုဂ်", "ဃ္", "ဃ်", "ာဃ္", "ာဃ်", "ါဃ္", "ါဃ်", "ိဃ္", "ိဃ်", "ုဃ္", "ုဃ်", "ေဃ္", "ေဃ်", "ောဃ္", "ောဃ်", "ေါဃ္", "ေါဃ်", "ိုဃ္", "ိုဃ်", "င်္", "င်", "ာင်္", "ာင်", "ါင်္", "ါင်", "ိင်္", "ိင်", "ုင်္", "ုင်", "ေင်္", "ေင်", "ောင်္", "ောင်", "ေါင်္", "ေါင်", "ိုင်္", "ိုင်", "စ္", "စ်", "ာစ္", "ာစ်", "ါစ္", "ါစ်", "ိစ္", "ိစ်", "ုစ္", "ုစ်", "ေစ္", "ေစ်", "ောစ္", "ောစ်", "ေါစ္", "ေါစ်", "ိုစ္", "ိုစ်", "ဆ္", "ဆ်", "ာဆ္", "ာဆ်", "ါဆ္", "ါဆ်", "ိဆ္", "ိဆ်", "ုဆ္", "ုဆ်", "ေဆ္", "ေဆ်", "ောဆ္", "ောဆ်", "ေါဆ္", "ေါဆ်", "ိုဆ္", "ိုဆ်", "ဇ္", "ဇ်", "ာဇ္", "ာဇ်", "ါဇ္", "ါဇ်", "ိဇ္", "ိဇ်", "ုဇ္", "ုဇ်", "ေဇ္", "ေဇ်", "ောဇ္", "ောဇ်", "ေါဇ္", "ေါဇ်", "ိုဇ္", "ိုဇ်", "ဈ်", "ာဈ်", "ါဈ်", "ိဈ်", "ုဈ်", "ေဈ်", "ောဈ်", "ေါဈ်", "ိုဈ်", "ဉ္", "ဉ်", "ာဉ္", "ာဉ်", "ါဉ္", "ါဉ်", "ိဉ္", "ိဉ်", "ုဉ္", "ုဉ်", "ေဉ္", "ေဉ်", "ောဉ္", "ောဉ်", "ေါဉ္", "ေါဉ်", "ိုဉ္", "ိုဉ်", "ည္", "ည်", "ာည္", "ာည်", "ါည္", "ါည်", "ိည္", "ိည်", "ုည္", "ုည်", "ေည္", "ေည်", "ောည္", "ောည်", "ေါည္", "ေါည်", "ိုည္", "ိုည်", "ဋ္", "ဋ်", "ာဋ္", "ာဋ်", "ါဋ္", "ါဋ်", "ိဋ္", "ိဋ်", "ုဋ္", "ုဋ်", "ေဋ္", "ေဋ်", "ောဋ္", "ောဋ်", "ေါဋ္", "ေါဋ်", "ိုဋ္", "ိုဋ်", "ဌ္", "ဌ်", "ာဌ္", "ာဌ်", "ါဌ္", "ါဌ်", "ိဌ္", "ိဌ်", "ုဌ္", "ုဌ်", "ေဌ္", "ေဌ်", "ောဌ္", "ောဌ်", "ေါဌ္", "ေါဌ်", "ိုဌ္", "ိုဌ်", "ဍ္", "ဍ်", "ာဍ္", "ာဍ်", "ါဍ္", "ါဍ်", "ိဍ္", "ိဍ်", "ုဍ္", "ုဍ်", "ေဍ္", "ေဍ်", "ောဍ္", "ောဍ်", "ေါဍ္", "ေါဍ်", "ိုဍ္", "ိုဍ်", "ဎ္", "ဎ်", "ာဎ္", "ာဎ်", "ါဎ္", "ါဎ်", "ိဎ္", "ိဎ်", "ုဎ္", "ုဎ်", "ေဎ္", "ေဎ်", "ောဎ္", "ောဎ်", "ေါဎ္", "ေါဎ်", "ိုဎ္", "ိုဎ်", "ဏ္", "ဏ်", "ာဏ္", "ာဏ်", "ါဏ္", "ါဏ်", "ိဏ္", "ိဏ်", "ုဏ္", "ုဏ်", "ေဏ္", "ေဏ်", "ောဏ္", "ောဏ်", "ေါဏ္", "ေါဏ်", "ိုဏ္", "ိုဏ်", "တ္", "တ်", "ာတ္", "ာတ်", "ါတ္", "ါတ်", "ိတ္", "ိတ်", "ုတ္", "ုတ်", "ေတ္", "ေတ်", "ောတ္", "ောတ်", "ေါတ္", "ေါတ်", "ိုတ္", "ိုတ်", "ထ္", "ထ်", "ာထ္", "ာထ်", "ါထ္", "ါထ်", "ိထ္", "ိထ်", "ုထ္", "ုထ်", "ေထ္", "ေထ်", "ောထ္", "ောထ်", "ေါထ္", "ေါထ်", "ိုထ္", "ိုထ်", "ဒ္", "ဒ်", "ာဒ္", "ာဒ်", "ါဒ္", "ါဒ်", "ိဒ္", "ိဒ်", "ုဒ္", "ုဒ်", "ေဒ္", "ေဒ်", "ောဒ္", "ောဒ်", "ေါဒ္", "ေါဒ်", "ိုဒ္", "ိုဒ်", "ဓ္", "ဓ်", "ာဓ္", "ာဓ်", "ါဓ္", "ါဓ်", "ိဓ္", "ိဓ်", "ုဓ္", "ုဓ်", "ေဓ္", "ေဓ်", "ောဓ္", "ောဓ်", "ေါဓ္", "ေါဓ်", "ိုဓ္", "ိုဓ်", "န္", "န်", "န်ုပ်", "ာန္", "ာန်", "ါန္", "ါန်", "ိန္", "ိန်", "ုန္", "ုန်", "ေန္", "ေန်", "ောန္", "ောန်", "ေါန္", "ေါန်", "ိုန္", "ိုန်", "ပ္", "ပ်", "ာပ္", "ာပ်", "ါပ္", "ါပ်", "ိပ္", "ိပ်", "ုပ္", "ုပ်", "ေပ္", "ေပ်", "ောပ္", "ောပ်", "ေါပ္", "ေါပ်", "ိုပ္", "ိုပ်", "ဖ္", "ဖ်", "ာဖ္", "ာဖ်", "ါဖ္", "ါဖ်", "ိဖ္", "ိဖ်", "ုဖ္", "ုဖ်", "ေဖ္", "ေဖ်", "ောဖ္", "ောဖ်", "ေါဖ္", "ေါဖ်", "ိုဖ္", "ိုဖ်", "ဗ္", "ဗ်", "ာဗ္", "ာဗ်", "ါဗ္", "ါဗ်", "ိဗ္", "ိဗ်", "ုဗ္", "ုဗ်", "ေဗ္", "ေဗ်", "ောဗ္", "ောဗ်", "ေါဗ္", "ေါဗ်", "ိုဗ္", "ိုဗ်", "ဘ္", "ဘ်", "ာဘ္", "ာဘ်", "ါဘ္", "ါဘ်", "ိဘ္", "ိဘ်", "ုဘ္", "ုဘ်", "ေဘ္", "ေဘ်", "ောဘ္", "ောဘ်", "ေါဘ္", "ေါဘ်", "ိုဘ္", "ိုဘ်", "မ္", "မ်", "ာမ္", "ာမ်", "ါမ္", "ါမ်", "ိမ္", "ိမ်", "ိံ", "ိမ့်", "ိံ့", "ိမ်း", "ိံး", "ုမ္", "ုမ်", "ုံ", "ုမ့်", "ုံ့", "ုမ်း", "ုံး", "ေမ္", "ေမ်", "ောမ္", "ောမ်", "ေါမ္", "ေါမ်", "ိုမ္", "ိုမ်", "ယ္", "ယ်", "ာယ္", "ာယ်", "ါယ္", "ါယ်", "ိယ္", "ိယ်", "ုယ္", "ုယ်", "ေယ္", "ေယ်", "ောယ္", "ောယ်", "ေါယ္", "ေါယ်", "ိုယ္", "ိုယ်", "ရ္", "ရ်", "ာရ္", "ာရ်", "ါရ္", "ါရ်", "ိရ္", "ိရ်", "ုရ္", "ုရ်", "ေရ္", "ေရ်", "ောရ္", "ောရ်", "ေါရ္", "ေါရ်", "ိုရ္", "ိုရ်", "လ္", "လ်", "ာလ္", "ာလ်", "ါလ္", "ါလ်", "ိလ္", "ိလ်", "ုလ္", "ုလ်", "ေလ္", "ေလ်", "ောလ္", "ောလ်", "ေါလ္", "ေါလ်", "ိုလ္", "ိုလ်", "ဝ်", "ာဝ်", "ါဝ်", "ိဝ်", "ုဝ်", "ေဝ်", "ောဝ်", "ေါဝ်", "ိုဝ်", "သ္", "သ်", "ဿ", "ာသ္", "ာသ်", "ါသ္", "ါသ်", "ိသ္", "ိသ်", "ိဿ", "ုသ္", "ုသ်", "ုဿ", "ေသ္", "ေသ်", "ေဿ", "ောသ္", "ောသ်", "ေါသ္", "ေါသ်", "ိုသ္", "ိုသ်", "ဟ္", "ဟ်", "ာဟ္", "ာဟ်", "ါဟ္", "ါဟ်", "ိဟ္", "ိဟ်", "ုဟ္", "ုဟ်", "ေဟ္", "ေဟ်", "ောဟ္", "ောဟ်", "ေါဟ္", "ေါဟ်", "ိုဟ္", "ိုဟ်", "ဠ်", "ာဠ်", "ါဠ်", "ိဠ်", "ုဠ်", "ေဠ်", "ောဠ်", "ေါဠ်", "ိုဠ်", "အ်", "ာအ်", "ါအ်", "ိအ်", "ုအ်", "ေအ်", "ောအ်", "ေါအ်", "ိုအ်", "ၞ", "ၟ", "ျ", "ြ", "ၠ", "ွ", "ႂ", "ှ", "ျွ", "ြွ", "ျှ", "ြှ", "ွှ", "ျွှ", "ြွှ", "္", "်", "့", "း", "့း"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "chr";
		vector<string> characters {"ꭰ", "Ꭰ", "ꭱ", "Ꭱ", "ꭲ", "Ꭲ", "ꭳ", "Ꭳ", "ꭴ", "Ꭴ", "ꭵ", "Ꭵ", "ꭶ", "Ꭶ", "ꭷ", "Ꭷ", "ꭸ", "Ꭸ", "ꭹ", "Ꭹ", "ꭺ", "Ꭺ", "ꭻ", "Ꭻ", "ꭼ", "Ꭼ", "ꭽ", "Ꭽ", "ꭾ", "Ꭾ", "ꭿ", "Ꭿ", "ꮀ", "Ꮀ", "ꮁ", "Ꮁ", "ꮂ", "Ꮂ", "ꮃ", "Ꮃ", "ꮄ", "Ꮄ", "ꮅ", "Ꮅ", "ꮆ", "Ꮆ", "ꮇ", "Ꮇ", "ꮈ", "Ꮈ", "ꮉ", "Ꮉ", "ꮊ", "Ꮊ", "ꮋ", "Ꮋ", "ꮌ", "Ꮌ", "ꮍ", "Ꮍ", "ꮎ", "Ꮎ", "ꮏ", "Ꮏ", "ꮐ", "Ꮐ", "ꮑ", "Ꮑ", "ꮒ", "Ꮒ", "ꮓ", "Ꮓ", "ꮔ", "Ꮔ", "ꮕ", "Ꮕ", "ꮖ", "Ꮖ", "ꮗ", "Ꮗ", "ꮘ", "Ꮘ", "ꮙ", "Ꮙ", "ꮚ", "Ꮚ", "ꮛ", "Ꮛ", "ꮜ", "Ꮜ", "ꮝ", "Ꮝ", "ꮞ", "Ꮞ", "ꮟ", "Ꮟ", "ꮠ", "Ꮠ", "ꮡ", "Ꮡ", "ꮢ", "Ꮢ", "ꮣ", "Ꮣ", "ꮤ", "Ꮤ", "ꮥ", "Ꮥ", "ꮦ", "Ꮦ", "ꮧ", "Ꮧ", "ꮨ", "Ꮨ", "ꮩ", "Ꮩ", "ꮪ", "Ꮪ", "ꮫ", "Ꮫ", "ꮬ", "Ꮬ", "ꮭ", "Ꮭ", "ꮮ", "Ꮮ", "ꮯ", "Ꮯ", "ꮰ", "Ꮰ", "ꮱ", "Ꮱ", "ꮲ", "Ꮲ", "ꮳ", "Ꮳ", "ꮴ", "Ꮴ", "ꮵ", "Ꮵ", "ꮶ", "Ꮶ", "ꮷ", "Ꮷ", "ꮸ", "Ꮸ", "ꮹ", "Ꮹ", "ꮺ", "Ꮺ", "ꮻ", "Ꮻ", "ꮼ", "Ꮼ", "ꮽ", "Ꮽ", "ꮾ", "Ꮾ", "ꮿ", "Ꮿ", "ᏸ", "Ᏸ", "ᏹ", "Ᏹ", "ᏺ", "Ᏺ", "ᏻ", "Ᏻ", "ᏼ", "Ᏼ"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "hr";
		vector<string> characters {"a", "A", "b", "B", "c", "C", "č", "Č", "ç̌", "Ç̌", "ć", "Ć", "ḉ", "Ḉ", "d", "D", "dž", "ǆ", "Dž", "ǅ", "DŽ", "Ǆ", "đ", "Đ", "e", "E", "f", "F", "g", "G", "h", "H", "i", "I", "j", "J", "k", "K", "l", "L", "lj", "ǉ", "Lj", "ǈ", "LJ", "Ǉ", "lĵ", "Lĵ", "LĴ", "lǰ", "Lǰ", "m", "M", "n", "N", "nj", "ǌ", "Nj", "ǋ", "NJ", "Ǌ", "nĵ", "Nĵ", "NĴ", "nǰ", "Nǰ", "o", "O", "p", "P", "r", "R", "s", "S", "š", "Š", "ṧ", "Ṧ", "ş̌", "Ş̌", "ṣ̌", "Ṣ̌", "ș̌", "Ș̌", "t", "T", "u", "U", "v", "V", "z", "Z", "ž", "Ž", "ẓ̌", "Ẓ̌", "ẕ̌", "Ẕ̌", "ﺎ", "ﺍ", "آ", "ﺂ", "ﺁ", "أ", "ﺄ", "ﺃ", "إ", "ﺈ", "ﺇ", "ﻫ", "ﻬ", "ﻪ", "ﻩ", "ة", "ﺔ", "ﺓ", "ۥ", "ﻮ", "ﻭ", "ؤ", "ﺆ", "ﺅ", "ۦ", "ﻳ", "ﻴ", "ﻲ", "ﻱ", "ئ", "ﺋ", "ﺌ", "ﺊ", "ﺉ", "ى", "ﯨ", "ﯩ", "ﻰ", "ﻯ", "เก", "เข", "เฃ", "เค", "เฅ", "เฆ", "เง", "เจ", "เฉ", "เช", "เซ", "เฌ", "เญ", "เฎ", "เฏ", "เฐ", "เฑ", "เฒ", "เณ", "เด", "เต", "เถ", "เท", "เธ", "เน", "เบ", "เป", "เผ", "เฝ", "เพ", "เฟ", "เภ", "เม", "เย", "เร", "เฤ", "เล", "เฦ", "เว", "เศ", "เษ", "เส", "เห", "เฬ", "เอ", "เฮ", "แก", "แข", "แฃ", "แค", "แฅ", "แฆ", "แง", "แจ", "แฉ", "แช", "แซ", "แฌ", "แญ", "แฎ", "แฏ", "แฐ", "แฑ", "แฒ", "แณ", "แด", "แต", "แถ", "แท", "แธ", "แน", "แบ", "แป", "แผ", "แฝ", "แพ", "แฟ", "แภ", "แม", "แย", "แร", "แฤ", "แล", "แฦ", "แว", "แศ", "แษ", "แส", "แห", "แฬ", "แอ", "แฮ", "โก", "โข", "โฃ", "โค", "โฅ", "โฆ", "โง", "โจ", "โฉ", "โช", "โซ", "โฌ", "โญ", "โฎ", "โฏ", "โฐ", "โฑ", "โฒ", "โณ", "โด", "โต", "โถ", "โท", "โธ", "โน", "โบ", "โป", "โผ", "โฝ", "โพ", "โฟ", "โภ", "โม", "โย", "โร", "โฤ", "โล", "โฦ", "โว", "โศ", "โษ", "โส", "โห", "โฬ", "โอ", "โฮ", "ใก", "ใข", "ใฃ", "ใค", "ใฅ", "ใฆ", "ใง", "ใจ", "ใฉ", "ใช", "ใซ", "ใฌ", "ใญ", "ใฎ", "ใฏ", "ใฐ", "ใฑ", "ใฒ", "ใณ", "ใด", "ใต", "ใถ", "ใท", "ใธ", "ใน", "ใบ", "ใป", "ใผ", "ใฝ", "ใพ", "ใฟ", "ใภ", "ใม", "ใย", "ใร", "ใฤ", "ใล", "ใฦ", "ใว", "ใศ", "ใษ", "ใส", "ให", "ใฬ", "ใอ", "ใฮ", "ไก", "ไข", "ไฃ", "ไค", "ไฅ", "ไฆ", "ไง", "ไจ", "ไฉ", "ไช", "ไซ", "ไฌ", "ไญ", "ไฎ", "ไฏ", "ไฐ", "ไฑ", "ไฒ", "ไณ", "ได", "ไต", "ไถ", "ไท", "ไธ", "ไน", "ไบ", "ไป"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "en_US";
		vector<string> characters {"A", "Á", "À", "Ă", "Ắ", "Ằ", "Ẵ", "Ẳ", "Â", "Ấ", "Ầ", "Ẫ", "Ẩ", "Ǎ", "Å", "Ǻ", "Ä", "Ǟ", "Ã", "Ȧ", "Ǡ", "Ą", "Ā", "Ả", "Ȁ", "Ȃ", "Ạ", "Ặ", "Ậ", "Ḁ", "B", "Ḃ", "Ḅ", "Ḇ", "C", "Ć", "Ĉ", "Č", "Ċ", "Ç", "Ḉ", "D", "Ď", "Ḋ", "Ḑ", "Ḍ", "Ḓ", "Ḏ", "E", "É", "È", "Ĕ", "Ê", "Ế", "Ề", "Ễ", "Ể", "Ě", "Ë", "Ẽ", "Ė", "Ȩ", "Ḝ", "Ę", "Ē", "Ḗ", "Ḕ", "Ẻ", "Ȅ", "Ȇ", "Ẹ", "Ệ", "Ḙ", "Ḛ", "F", "Ḟ", "G", "Ǵ", "Ğ", "Ĝ", "Ǧ", "Ġ", "Ģ", "Ḡ", "H", "Ĥ", "Ȟ", "Ḧ", "Ḣ", "Ḩ", "Ḥ", "Ḫ", "I", "Í", "Ì", "Ĭ", "Î", "Ǐ", "Ï", "Ḯ", "Ĩ", "İ", "Į", "Ī", "Ỉ", "Ȉ", "Ȋ", "Ị", "Ḭ", "J", "Ĵ", "K", "Ḱ", "Ǩ", "Ķ", "Ḳ", "Ḵ", "L", "Ĺ", "Ľ", "Ļ", "Ḷ", "Ḹ", "Ḽ", "Ḻ", "M", "Ḿ", "Ṁ", "Ṃ", "N", "Ń", "Ǹ", "Ň", "Ñ", "Ṅ", "Ņ", "Ṇ", "Ṋ", "Ṉ", "O", "Ó", "Ò", "Ŏ", "Ô", "Ố", "Ồ", "Ỗ", "Ổ", "Ǒ", "Ö", "Ȫ", "Ő", "Õ", "Ṍ", "Ṏ", "Ȭ", "Ȯ", "Ȱ", "Ǫ", "Ǭ", "Ō", "Ṓ", "Ṑ", "Ỏ", "Ȍ", "Ȏ", "Ơ", "Ớ", "Ờ", "Ỡ", "Ở", "Ợ", "Ọ", "Ộ", "P", "Ṕ", "Ṗ", "Q", "R", "Ŕ", "Ř", "Ṙ", "Ŗ", "Ȑ", "Ȓ", "Ṛ", "Ṝ", "Ṟ", "S", "Ś", "Ṥ", "Ŝ", "Š", "Ṧ", "Ṡ", "Ş", "Ṣ", "Ṩ", "Ș", "T", "Ť", "Ṫ", "Ţ", "Ṭ", "Ț", "Ṱ", "Ṯ", "U", "Ú", "Ù", "Ŭ", "Û", "Ǔ", "Ů", "Ü", "Ǘ", "Ǜ", "Ǚ", "Ǖ", "Ű", "Ũ", "Ṹ", "Ų", "Ū", "Ṻ", "Ủ", "Ȕ", "Ȗ", "Ư", "Ứ", "Ừ", "Ữ", "Ử", "Ự", "Ụ", "Ṳ", "Ṷ", "Ṵ", "V", "Ṽ", "Ṿ", "W", "Ẃ", "Ẁ", "Ŵ", "Ẅ", "Ẇ", "Ẉ", "X", "Ẍ", "Ẋ", "Y", "Ý", "Ỳ", "Ŷ", "Ÿ", "Ỹ", "Ẏ", "Ȳ", "Ỷ", "Ỵ", "Z", "Ź", "Ẑ", "Ž", "Ż", "Ẓ", "Ẕ"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "fi";
		vector<string> characters {"a", "A", "b", "B", "c", "C", "d", "D", "đ", "Đ", "e", "E", "f", "F", "g", "G", "ǥ", "Ǥ", "h", "H", "i", "I", "j", "J", "k", "K", "l", "L", "m", "M", "n", "N", "ŋ", "Ŋ", "o", "O", "p", "P", "q", "Q", "r", "R", "s", "S", "š", "Š", "t", "T", "ŧ", "Ŧ", "u", "U", "v", "V", "w", "W", "x", "X", "y", "Y", "ü", "Ü", "ǘ", "Ǘ", "ǜ", "Ǜ", "ǚ", "Ǚ", "ų̈", "Ų̈", "ǖ", "Ǖ", "ư̈", "Ư̈", "ự̈", "Ự̈", "ụ̈", "Ụ̈", "ṳ̈", "Ṳ̈", "ṷ̈", "Ṷ̈", "ṵ̈", "Ṵ̈", "z", "Z", "ž", "Ž", "ʒ", "Ʒ", "ǯ", "Ǯ", "å", "Å", "ǻ", "Ǻ", "ą̊", "Ą̊", "ạ̊", "Ạ̊", "ḁ̊", "Ḁ̊", "ä", "Ä", "ą̈", "Ą̈", "ǟ", "Ǟ", "ạ̈", "Ạ̈", "ḁ̈", "Ḁ̈", "æ", "Æ", "ǽ", "Ǽ", "ǣ", "Ǣ", "ö", "Ö", "ǫ̈", "Ǫ̈", "ȫ", "Ȫ", "ơ̈", "Ơ̈", "ợ̈", "Ợ̈", "ọ̈", "Ọ̈", "ø", "Ø", "ǿ", "Ǿ"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "el";
		vector<string> characters {"α", "Α", "ά", "Ά", "β", "ϐ", "Β", "γ", "Γ", "δ", "Δ", "ε", "ϵ", "Ε", "έ", "Έ", "ζ", "Ζ", "η", "Η", "ή", "Ή", "θ", "ϑ", "Θ", "ϴ", "ι", "Ι", "ί", "Ί", "ϊ", "Ϊ", "ΐ", "κ", "ϰ", "Κ", "λ", "Λ", "μ", "µ", "Μ", "ν", "Ν", "ξ", "Ξ", "ο", "Ο", "ό", "Ό", "π", "ϖ", "Π", "ρ", "ϱ", "Ρ", "σ", "Σ", "ς", "τ", "Τ", "υ", "Υ", "ύ", "Ύ", "ϋ", "Ϋ", "ΰ", "φ", "ϕ", "Φ", "χ", "Χ", "ψ", "Ψ", "ω", "Ω", "ώ", "Ώ"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "ro";
		vector<string> characters {"a", "A", "ă", "Ă", "ắ", "Ắ", "ằ", "Ằ", "ẵ", "Ẵ", "ą̆", "Ą̆", "ẳ", "Ẳ", "ặ", "Ặ", "ḁ̆", "Ḁ̆", "â", "Â", "ấ", "Ấ", "ầ", "Ầ", "ẫ", "Ẫ", "ą̂", "Ą̂", "ẩ", "Ẩ", "ậ", "Ậ", "ḁ̂", "Ḁ̂", "b", "B", "c", "C", "d", "D", "e", "E", "f", "F", "g", "G", "h", "H", "i", "I", "î", "Î", "į̂", "Į̂", "ị̂", "Ị̂", "ḭ̂", "Ḭ̂", "j", "J", "k", "K", "l", "L", "m", "M", "n", "N", "o", "O", "p", "P", "r", "R", "s", "S", "ș", "ş", "Ș", "Ş", "t", "T", "ț", "ţ", "Ț", "Ţ", "u", "U", "v", "V", "x", "X", "z", "Z"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "uk";
		vector<string> characters {"а", "А", "б", "Б", "в", "В", "г", "Г", "ґ", "Ґ", "д", "Д", "е", "Е", "є", "Є", "ж", "Ж", "з", "З", "и", "И", "і", "І", "ї", "ꙶ", "Ї", "й", "Й", "к", "К", "л", "Л", "м", "М", "н", "Н", "о", "О", "п", "П", "р", "Р", "с", "С", "т", "Т", "у", "У", "ф", "Ф", "х", "Х", "ц", "Ц", "ч", "Ч", "ш", "Ш", "щ", "Щ", "ь", "Ь", "ю", "Ю", "я", "Я", "ʼ"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "es";
		vector<string> characters {"a", "A", "á", "Á", "b", "B", "c", "C", "d", "D", "e", "E", "é", "É", "f", "F", "g", "G", "h", "H", "i", "I", "í", "Í", "j", "J", "k", "K", "l", "L", "m", "M", "n", "N", "ñ", "Ñ", "ņ̃", "Ņ̃", "ṇ̃", "Ṇ̃", "ṋ̃", "Ṋ̃", "ṉ̃", "Ṉ̃", "o", "O", "ó", "Ó", "p", "P", "q", "Q", "r", "R", "s", "S", "t", "T", "u", "U", "ú", "Ú", "ü", "Ü", "v", "V", "w", "W", "x", "X", "y", "Y", "z", "Z"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "pl";
		vector<string> characters {"a", "A", "ą", "Ą", "b", "B", "c", "C", "ć", "Ć", "ḉ", "Ḉ", "d", "D", "e", "E", "ę", "Ę", "f", "F", "g", "G", "h", "H", "i", "I", "j", "J", "k", "K", "l", "L", "ł", "Ł", "m", "M", "n", "N", "ń", "Ń", "ņ́", "Ņ́", "ṇ́", "Ṇ́", "ṋ́", "Ṋ́", "ṉ́", "Ṉ́", "o", "O", "ó", "Ó", "ǫ́", "Ǫ́", "ớ", "Ớ", "ợ́", "Ợ́", "ọ́", "Ọ́", "p", "P", "r", "R", "s", "S", "ś", "Ś", "ṥ", "Ṥ", "ş́", "Ş́", "ṣ́", "Ṣ́", "ș́", "Ș́", "t", "T", "u", "U", "w", "W", "y", "Y", "z", "Z", "ź", "Ź", "ẓ́", "Ẓ́", "ẕ́", "Ẕ́", "ż", "Ż", "ẓ̇", "Ẓ̇", "ẕ̇", "Ẕ̇"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "ko";
		vector<string> characters {"伽", "佳", "假", "價", "加", "可", "呵", "哥", "嘉", "嫁", "家", "暇", "架", "枷", "柯", "歌", "珂", "痂", "稼", "苛", "茄", "街", "袈", "訶", "賈", "跏", "軻", "迦", "駕"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "sv";
		vector<string> characters {"a", "A", "à", "À", "b", "B", "c", "C", "d", "D", "đ", "Đ", "ð", "Ð", "e", "E", "é", "É", "f", "F", "g", "G", "h", "H", "i", "I", "j", "J", "k", "K", "l", "L", "m", "M", "n", "N", "o", "O", "p", "P", "q", "Q", "r", "R", "s", "S", "t", "T", "þ", "Þ", "u", "U", "v", "V", "Ṽ", "Ṿ", "w", "W", "ẃ", "Ẃ", "ẁ", "Ẁ", "ŵ", "Ŵ", "ẘ", "ẅ", "Ẅ", "ẇ", "Ẇ", "ẉ", "Ẉ", "x", "X", "y", "Y", "ü", "Ü", "ǘ", "Ǘ", "ǜ", "Ǜ", "ǚ", "Ǚ", "ų̈", "Ų̈", "ǖ", "Ǖ", "ư̈", "Ư̈", "ự̈", "Ự̈", "ụ̈", "Ụ̈", "ṳ̈", "Ṳ̈", "ṷ̈", "Ṷ̈", "ṵ̈", "Ṵ̈", "ű", "Ű", "ų̋", "Ų̋", "ư̋", "Ư̋", "ự̋", "Ự̋", "ụ̋", "Ụ̋", "ṳ̋", "Ṳ̋", "ṷ̋", "Ṷ̋", "ṵ̋", "Ṵ̋", "z", "Z", "å", "Å", "ǻ", "Ǻ", "ą̊", "Ą̊", "ạ̊", "Ạ̊", "ḁ̊", "Ḁ̊", "ä", "Ä", "ą̈", "Ą̈", "ǟ", "Ǟ", "ạ̈", "Ạ̈", "ḁ̈", "Ḁ̈", "æ", "Æ", "ǽ", "Ǽ", "ǣ", "Ǣ", "ę", "Ę", "ö", "Ö", "ǫ̈", "Ǫ̈", "ȫ", "Ȫ", "ơ̈", "Ơ̈", "ợ̈", "Ợ̈", "ọ̈", "Ọ̈", "ø", "Ø", "ǿ", "Ǿ", "ő", "Ő", "ǫ̋", "Ǫ̋", "ơ̋", "Ơ̋", "ợ̋", "Ợ̋", "ọ̋", "Ọ̋", "œ", "Œ", "ô", "Ô", "ố", "Ố", "ồ", "Ồ", "ỗ", "Ỗ", "ǫ̂", "Ǫ̂", "ổ", "Ổ", "ơ̂", "Ơ̂", "ợ̂", "Ợ̂", "ộ", "Ộ"};
		TestCollation(con, collation, characters);
	}

	{
		string collation = "fr_CA";
		vector<string> characters {"a", "A", "à", "À", "â", "Â", "æ", "Æ", "b", "B", "c", "C", "ç", "Ç", "d", "D", "e", "E", "é", "É", "è", "È", "ê", "Ê", "ë", "Ë", "f", "F", "g", "G", "h", "H", "i", "I", "î", "Î", "ï", "Ï", "j", "J", "k", "K", "l", "L", "m", "M", "n", "N", "o", "O", "ô", "Ô", "œ", "Œ", "p", "P", "q", "Q", "r", "R", "s", "S", "t", "T", "u", "U", "ù", "Ù", "û", "Û", "ü", "Ü", "v", "V", "w", "W", "x", "X", "y", "Y", "ÿ", "Ÿ", "z", "Z"};
		TestCollation(con, collation, characters);
	}
}
