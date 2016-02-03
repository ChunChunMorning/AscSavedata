# pragma once
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

		class CSavedata;

		std::shared_ptr<CSavedata> pImpl;

		String getString(const String& key) const;

		void addString(const String& key, const String& value);

	public:

		/// <summary>
		/// �f�t�H���g�R���X�g���N�^
		/// </summary>
		Savedata();

		/// <summary>
		/// �Z�[�u�f�[�^���쐬
		/// </summary>
		/// <param name="aes128Key">
		/// AES-128�Í���
		/// </param>
		Savedata(const AES128Key& aes128Key);

		/// <summary>
		/// �Z�[�u�f�[�^���쐬
		/// </summary>
		/// <param name="aes128Key">
		/// AES-128�Í���
		/// </param>
		/// <param name="delimiter">
		/// ��؂蕶��
		/// </param>
		Savedata(const AES128Key& aes128Key, wchar delimiter);

		/// <summary>
		/// �w�肵���L�[�̑��݂��m�F
		/// </summary>
		/// <param name="key">
		/// �w�肷��L�[
		/// </param>
		/// <returns>
		/// �L�[�������true
		/// </returns>
		bool hasKey(const String& key) const;

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
				return ParseOpt<Type>(getString(key));
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
			addString(key, Format(value));
		}

		/// <summary>
		/// �w�肵���L�[���폜
		/// </summary>
		/// <returns>
		/// �폜�����L�[�̐�
		/// </returns>
		size_t erase(const String& key);

		/// <summary>
		/// �L�[��S�č폜
		/// </summary>
		/// <returns>
		/// �Ȃ�
		/// </returns>
		void clear();

		/// <summary>
		/// �Ō�ɓǂݏ��������f�[�^�����[�h
		/// </summary>
		/// <returns>
		/// ���[�h�ɐ��������true
		/// </returns>
		bool load();

		/// <summary>
		/// �w�肵���t�@�C���p�X�̃f�[�^�����[�h
		/// </summary>
		/// <param name="filePath">
		/// ���[�h����f�[�^�̃t�@�C���p�X
		/// </param>
		/// <returns>
		/// ���[�h�ɐ��������true
		/// </returns>
		bool load(const FilePath& filePath);

		/// <summary>
		/// �Ō�ɓǂݏ��������e�L�X�g�f�[�^�����[�h
		/// </summary>
		/// <returns>
		/// ���[�h�ɐ��������true
		/// </returns>
		bool loadByPlaneText();

		/// <summary>
		/// �w�肵���t�@�C���p�X�̃f�[�^�����[�h
		/// </summary>
		/// <param name="filePath">
		/// ���[�h����e�L�X�g�f�[�^�̃t�@�C���p�X
		/// </param>
		/// <returns>
		/// ���[�h�ɐ��������true
		/// </returns>
		bool loadByPlaneText(const FilePath& filePath);

		/// <summary>
		/// �Ō�ɓǂݏ��������t�@�C���p�X�Ƀf�[�^���Z�[�u
		/// </summary>
		/// <returns>
		/// �Z�[�u�ɐ��������true
		/// </returns>
		bool save();

		/// <summary>
		/// �w�肵���t�@�C���p�X�Ƀf�[�^���Z�[�u
		/// </summary>
		/// <param name="filePath">
		/// �Z�[�u����f�[�^�̃t�@�C���p�X
		/// </param>
		/// <returns>
		/// �Z�[�u�ɐ��������true
		/// </returns>
		bool save(const FilePath& filePath);

		/// <summary>
		/// �Ō�ɓǂݏ��������t�@�C���p�X�Ƀe�L�X�g�f�[�^���Z�[�u
		/// </summary>
		/// <returns>
		/// �Z�[�u�ɐ��������true
		/// </returns>
		bool saveByPlaneText();

		/// <summary>
		/// �w�肵���t�@�C���p�X�Ƀe�L�X�g�f�[�^���Z�[�u
		/// </summary>
		/// <param name="filePath">
		/// �Z�[�u����f�[�^�̃t�@�C���p�X
		/// </param>
		/// <returns>
		/// �Z�[�u�ɐ��������true
		/// </returns>
		bool saveByPlaneText(const FilePath& filePath);
	};
}
