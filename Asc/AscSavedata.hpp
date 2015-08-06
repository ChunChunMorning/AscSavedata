# pragma once
# include <unordered_map>
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

		wchar m_delimiter;

		AES128Key m_aes128Key;

		std::unordered_map<String, String> m_data;

		FilePath m_lastPath;

		FilePath m_makeFilePath(const FilePath& path);

	public:

		/// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		Savedata() : m_delimiter(L'@') {}

		/// <summary>
		/// セーブデータを作成
		/// </summary>
		/// <param name="aes128Key">
		/// AES-128暗号鍵
		/// </param>
		Savedata(AES128Key aes128Key) : m_aes128Key(aes128Key), m_delimiter(L'@') {}

		/// <summary>
		/// セーブデータを作成
		/// </summary>
		/// <param name="aes128Key">
		/// AES-128暗号鍵
		/// </param>
		/// <param name="delimiter">
		/// 区切り文字
		/// </param>
		Savedata(AES128Key aes128Key, wchar delimiter) : m_aes128Key(aes128Key), m_delimiter(delimiter) {}

		/// <summary>
		/// セーブ時の区切り文字
		/// </summary>
		__declspec(property(get = _get_delimiter, put = _put_delimiter)) wchar delimiter;

		/// <summary>
		/// ロード、セーブに使うAES-128暗号鍵
		/// </summary>
		__declspec(property(get = _get_aes128Key, put = _put_aes128Key)) AES128Key aes128Key;

		/// <summary>
		/// 内部データ
		/// </summary>
		__declspec(property(get = _get_data, put = _put_data)) std::unordered_map<String, String> data;

		/// <summary>
		/// 指定したキーの存在を確認
		/// </summary>
		/// <param name="key">
		/// 指定するキー
		/// </param>
		/// <returns>
		/// キーがあればtrue
		/// </returns>
		bool hasKey(const String& key) const
		{
			return m_data.find(key) != m_data.end();
		}

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
				return ParseOpt<Type>(m_data.at(key));
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
			m_data[key] = Format(value);
		}

		/// <summary>
		/// 指定したキーを削除
		/// </summary>
		/// <returns>
		/// 削除したキーの数
		/// </returns>
		size_t erase(const String& key)
		{
			return m_data.erase(key);
		}

		/// <summary>
		/// キーを全て削除
		/// </summary>
		/// <returns>
		/// なし
		/// </returns>
		void clear()
		{
			m_data.clear();
		}

		/// <summary>
		/// 最後に読み書きしたデータをロード
		/// </summary>
		/// <returns>
		/// ロードに成功すればtrue
		/// </returns>
		bool load()
		{
#ifdef NOT_USING_AES
			TextReader reader(m_lastPath);

			if (!reader)
				return false;

			String data = reader.readContents();
#else
			BinaryReader reader(m_lastPath);

			if (!reader)
				return false;

			String data;

			if (!Crypto::DecryptString(reader.readWhole(), data, m_aes128Key))
				return false;
#endif
			auto contents = data.substr(1).split(data[0]);

			for (size_t i = 0; i < contents.size(); i += 2)
			{
				m_data[contents[i]] = contents[i + 1];
			}

			return true;
		}

		/// <summary>
		/// 指定したファイルパスのデータをロード
		/// </summary>
		/// <param name="filePath">
		/// ロードするデータのファイルパス
		/// </param>
		/// <returns>
		/// ロードに成功すればtrue
		/// </returns>
		bool load(const FilePath& filePath)
		{
#ifdef NOT_USING_APPDATA
			m_lastPath = filePath;
#else
			m_lastPath = m_makeFilePath(filePath);
#endif

#ifdef NOT_USING_AES
			TextReader reader(m_lastPath);

			if (!reader)
				return false;

			String data = reader.readContents();
#else
			BinaryReader reader(m_lastPath);

			if (!reader)
				return false;

			String data;

			if (!Crypto::DecryptString(reader.readWhole(), data, m_aes128Key))
				return false;
#endif
			auto contents = data.substr(1).split(data[0]);

			for (size_t i = 0; i < contents.size(); i += 2)
			{
				m_data[contents[i]] = contents[i + 1];
			}

			return true;
		}

		/// <summary>
		/// 最後に読み書きしたファイルパスにデータをセーブ
		/// </summary>
		/// <returns>
		/// セーブに成功すればtrue
		/// </returns>
		bool save()
		{
			String data = L"";

			for (auto content : m_data)
			{
				data += m_delimiter + content.first + m_delimiter + content.second;
			}
#ifdef NOT_USING_AES
			TextWriter writer(m_lastPath);

			writer.writeln(data);
#else
			const MemoryReader encrypted = Crypto::EncryptString(data, m_aes128Key);

			encrypted.save(m_lastPath);
#endif
			return true;
		}


		/// <summary>
		/// 指定したファイルパスにデータをセーブ
		/// </summary>
		/// <param name="filePath">
		/// セーブするデータのファイルパス
		/// </param>
		/// <returns>
		/// セーブに成功すればtrue
		/// </returns>
		bool save(const FilePath& filePath)
		{
#ifdef NOT_USING_APPDATA
			m_lastPath = filePath;
#else
			m_lastPath = m_makeFilePath(filePath);
#endif
			String data = L"";

			for (auto content : m_data)
			{
				data += m_delimiter + content.first + m_delimiter + content.second;
			}
#ifdef NOT_USING_AES
			TextWriter writer(m_lastPath);

			writer.writeln(data);
#else
			const MemoryReader encrypted = Crypto::EncryptString(data, m_aes128Key);

			encrypted.save(m_lastPath);
#endif
			return true;
		}

		/// <summary>
		/// プロパティ用
		/// </summary>
		wchar _get_delimiter() const { return m_delimiter; }

		/// <summary>
		/// プロパティ用
		/// </summary>
		void _put_delimiter(wchar value) { m_delimiter = value; }

		/// <summary>
		/// プロパティ用
		/// </summary>
		AES128Key _get_aes128Key() const { return m_aes128Key; }

		/// <summary>
		/// プロパティ用
		/// </summary>
		void _put_aes128Key(AES128Key value) { m_aes128Key = value; }

		/// <summary>
		/// プロパティ用
		/// </summary>
		std::unordered_map<String, String> _get_data() const { return m_data; }

		/// <summary>
		/// プロパティ用
		/// </summary>
		void _put_data(std::unordered_map<String, String> value) { m_data = value; }

	};
}
