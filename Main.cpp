// 定義するとAESで暗号化しない
# define NOT_USING_AES

// 定義するとそのままのファイルパスで保存
# define NOT_USING_APPDATA

# include <Siv3D.hpp>
# include "Asc/AscSavedata.hpp"

void Main()
{
	// セーブデータの作成
	asc::Savedata savedata(AES128Key(12, 34, 56, 78));

	// キーを追加する
	savedata.add(L"name", L"asachun");
	savedata.add(L"pos", Point(100, 100));
	savedata.add(L"hp", 100);

	// データをセーブ
	savedata.save(L"Asablo/Savedata.bin");

	WaitKey();

	// データをロード
	savedata.load();

	// 値の取り出し
	Println(savedata.get<String>(L"name"));
	Println(savedata.get<Point>(L"pos"));

	// キーの削除
	savedata.erase(L"hp");

	// パラメータの変更
	savedata.delimiter = L'$';
	savedata.aes128Key = AES128Key();

	WaitKey();
}
