# pragma once
# include <unordered_map>
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	/// <summary>
	/// �Z�[�u�f�[�^
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
		/// �f�t�H���g�R���X�g���N�^
		/// </summary>
		Savedata() : m_delimiter(L'@') {}

		/// <summary>
		/// �Z�[�u�f�[�^���쐬
		/// </summary>
		/// <param name="aes128Key">
		/// AES-128�Í���
		/// </param>
		Savedata(AES128Key aes128Key) : m_aes128Key(aes128Key), m_delimiter(L'@') {}

		/// <summary>
		/// �Z�[�u�f�[�^���쐬
		/// </summary>
		/// <param name="aes128Key">
		/// AES-128�Í���
		/// </param>
		/// <param name="delimiter">
		/// ��؂蕶��
		/// </param>
		Savedata(AES128Key aes128Key, wchar delimiter) : m_aes128Key(aes128Key), m_delimiter(delimiter) {}

		/// <summary>
		/// �Z�[�u���̋�؂蕶��
		/// </summary>
		__declspec(property(get = _get_delimiter, put = _put_delimiter)) wchar delimiter;

		/// <summary>
		/// ���[�h�A�Z�[�u�Ɏg��AES-128�Í���
		/// </summary>
		__declspec(property(get = _get_aes128Key, put = _put_aes128Key)) AES128Key aes128Key;

		/// <summary>
		/// �����f�[�^
		/// </summary>
		__declspec(property(get = _get_data, put = _put_data)) std::unordered_map<String, String> data;

		/// <summary>
		/// �w�肵���L�[�̑��݂��m�F
		/// </summary>
		/// <param name="key">
		/// �w�肷��L�[
		/// </param>
		/// <returns>
		/// �L�[�������true
		/// </returns>
		bool hasKey(const String& key) const
		{
			return m_data.find(key) != m_data.end();
		}

		/// <summary>
		/// �w�肵���L�[�̃f�[�^���擾
		/// </summary>
		/// <param name="key">
		/// �w�肷��L�[
		/// </param>
		/// <returns>
		/// �w�肵���L�[�̃f�[�^�B�ϊ��Ɏ��s�����ꍇ�̓f�t�H���g�̒l
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
		/// �w�肵���L�[�̃f�[�^���擾
		/// </summary>
		/// <param name="key">
		/// �w�肷��L�[
		/// </param>
		/// <param name="defaultValue">
		/// �ϊ��Ɏ��s�����Ƃ��ɕԂ��l
		/// </param>
		/// <returns>
		/// �w�肵���L�[�̃f�[�^�B�ϊ��Ɏ��s�����ꍇ�� defaultValue
		/// </returns>
		template<class Type> Type getOr(const String& key, Type&& defaultValue) const
		{
			return getOpt<Type>(key).value_or(std::move(defaultValue));
		}

		/// <summary>
		/// �w�肵���L�[�̃f�[�^���擾
		/// </summary>
		/// <param name="key">
		/// �w�肷��L�[
		/// </param>
		/// <returns>
		/// �w�肵���L�[�̃f�[�^�� Optional �^�B�ϊ��Ɏ��s�����ꍇ�� none
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
		/// �L�[��ǉ�
		/// </summary>
		/// <param name="key">
		/// �ǉ�����L�[
		/// </param>
		/// <param name="value">
		/// �ǉ�����L�[�̒l
		/// </param>
		/// <returns>
		/// �Ȃ�
		/// </returns>
		template<typename Type> void add(const String& key, const Type& value)
		{
			m_data[key] = Format(value);
		}

		/// <summary>
		/// �w�肵���L�[���폜
		/// </summary>
		/// <returns>
		/// �폜�����L�[�̐�
		/// </returns>
		size_t erase(const String& key)
		{
			return m_data.erase(key);
		}

		/// <summary>
		/// �L�[��S�č폜
		/// </summary>
		/// <returns>
		/// �Ȃ�
		/// </returns>
		void clear()
		{
			m_data.clear();
		}

		/// <summary>
		/// �Ō�ɓǂݏ��������f�[�^�����[�h
		/// </summary>
		/// <returns>
		/// ���[�h�ɐ��������true
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
		/// �w�肵���t�@�C���p�X�̃f�[�^�����[�h
		/// </summary>
		/// <param name="filePath">
		/// ���[�h����f�[�^�̃t�@�C���p�X
		/// </param>
		/// <returns>
		/// ���[�h�ɐ��������true
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
		/// �Ō�ɓǂݏ��������t�@�C���p�X�Ƀf�[�^���Z�[�u
		/// </summary>
		/// <returns>
		/// �Z�[�u�ɐ��������true
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
		/// �w�肵���t�@�C���p�X�Ƀf�[�^���Z�[�u
		/// </summary>
		/// <param name="filePath">
		/// �Z�[�u����f�[�^�̃t�@�C���p�X
		/// </param>
		/// <returns>
		/// �Z�[�u�ɐ��������true
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
		/// �v���p�e�B�p
		/// </summary>
		wchar _get_delimiter() const { return m_delimiter; }

		/// <summary>
		/// �v���p�e�B�p
		/// </summary>
		void _put_delimiter(wchar value) { m_delimiter = value; }

		/// <summary>
		/// �v���p�e�B�p
		/// </summary>
		AES128Key _get_aes128Key() const { return m_aes128Key; }

		/// <summary>
		/// �v���p�e�B�p
		/// </summary>
		void _put_aes128Key(AES128Key value) { m_aes128Key = value; }

		/// <summary>
		/// �v���p�e�B�p
		/// </summary>
		std::unordered_map<String, String> _get_data() const { return m_data; }

		/// <summary>
		/// �v���p�e�B�p
		/// </summary>
		void _put_data(std::unordered_map<String, String> value) { m_data = value; }

	};
}
