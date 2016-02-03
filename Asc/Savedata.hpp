# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	/// <summary>
	/// セーブデータ
	/// </summary>
	class Savedata
	{
	private:

		class CSavedata;

		std::shared_ptr<CSavedata> pImpl;

		String getString(const String& key) const;

		void addString(const String& key, const String& value);

	public:

		/// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		Savedata();

		/// <summary>
		/// セーブデータを作成
		/// </summary>
		/// <param name="aes128Key">
		/// AES-128暗号鍵
		/// </param>
		Savedata(const AES128Key& aes128Key);

		/// <summary>
		/// セーブデータを作成
		/// </summary>
		/// <param name="aes128Key">
		/// AES-128暗号鍵
		/// </param>
		/// <param name="delimiter">
		/// 区切り文字
		/// </param>
		Savedata(const AES128Key& aes128Key, wchar delimiter);

		/// <summary>
		/// 指定したキーの存在を確認
		/// </summary>
		/// <param name="key">
		/// 指定するキー
		/// </param>
		/// <returns>
		/// キーがあればtrue
		/// </returns>
		bool hasKey(const String& key) const;

		/// <summary>
		/// 指定したキーのデータを取得
		/// </summary>
		/// <param name="key">
		/// 指定するキー
		/// </param>
		/// <returns>
		/// 指定したキーのデータ。変換に失敗した場合はデフォルトの値
		/// </returns>
		template<class Type> Type get(const String& key) const
		{
			if (auto opt = getOpt<Type>(key))
			{
				return opt.value();
			}

			return Type();
		}

		/// <summary>
		/// 指定したキーのデータを取得
		/// </summary>
		/// <param name="key">
		/// 指定するキー
		/// </param>
		/// <param name="defaultValue">
		/// 変換に失敗したときに返す値
		/// </param>
		/// <returns>
		/// 指定したキーのデータ。変換に失敗した場合は defaultValue
		/// </returns>
		template<class Type> Type getOr(const String& key, Type&& defaultValue) const
		{
			return getOpt<Type>(key).value_or(std::move(defaultValue));
		}

		/// <summary>
		/// 指定したキーのデータを取得
		/// </summary>
		/// <param name="key">
		/// 指定するキー
		/// </param>
		/// <returns>
		/// 指定したキーのデータの Optional 型。変換に失敗した場合は none
		/// </returns>
		template<typename Type> Optional<Type> getOpt(String key) const
		{
			if (hasKey(key))
			{
				return ParseOpt<Type>(getString(key));
			}

			return none;
		}

		/// <summary>
		/// キーを追加
		/// </summary>
		/// <param name="key">
		/// 追加するキー
		/// </param>
		/// <param name="value">
		/// 追加するキーの値
		/// </param>
		/// <returns>
		/// なし
		/// </returns>
		template<typename Type> void add(const String& key, const Type& value)
		{
			addString(key, Format(value));
		}

		/// <summary>
		/// 指定したキーを削除
		/// </summary>
		/// <returns>
		/// 削除したキーの数
		/// </returns>
		size_t erase(const String& key);

		/// <summary>
		/// キーを全て削除
		/// </summary>
		/// <returns>
		/// なし
		/// </returns>
		void clear();

		/// <summary>
		/// 最後に読み書きしたデータをロード
		/// </summary>
		/// <returns>
		/// ロードに成功すればtrue
		/// </returns>
		bool load();

		/// <summary>
		/// 指定したファイルパスのデータをロード
		/// </summary>
		/// <param name="filePath">
		/// ロードするデータのファイルパス
		/// </param>
		/// <returns>
		/// ロードに成功すればtrue
		/// </returns>
		bool load(const FilePath& filePath);

		/// <summary>
		/// 最後に読み書きしたテキストデータをロード
		/// </summary>
		/// <returns>
		/// ロードに成功すればtrue
		/// </returns>
		bool loadByPlaneText();

		/// <summary>
		/// 指定したファイルパスのデータをロード
		/// </summary>
		/// <param name="filePath">
		/// ロードするテキストデータのファイルパス
		/// </param>
		/// <returns>
		/// ロードに成功すればtrue
		/// </returns>
		bool loadByPlaneText(const FilePath& filePath);

		/// <summary>
		/// 最後に読み書きしたファイルパスにデータをセーブ
		/// </summary>
		/// <returns>
		/// セーブに成功すればtrue
		/// </returns>
		bool save();

		/// <summary>
		/// 指定したファイルパスにデータをセーブ
		/// </summary>
		/// <param name="filePath">
		/// セーブするデータのファイルパス
		/// </param>
		/// <returns>
		/// セーブに成功すればtrue
		/// </returns>
		bool save(const FilePath& filePath);

		/// <summary>
		/// 最後に読み書きしたファイルパスにテキストデータをセーブ
		/// </summary>
		/// <returns>
		/// セーブに成功すればtrue
		/// </returns>
		bool saveByPlaneText();

		/// <summary>
		/// 指定したファイルパスにテキストデータをセーブ
		/// </summary>
		/// <param name="filePath">
		/// セーブするデータのファイルパス
		/// </param>
		/// <returns>
		/// セーブに成功すればtrue
		/// </returns>
		bool saveByPlaneText(const FilePath& filePath);
	};
}
