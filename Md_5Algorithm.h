#pragma once
#include "AlgorithmsBase.h"
#include "IHack.h"
#include "PasswordGenerator.h"
#include "openssl/sha.h"

class Md_5Algorithm : public AlgorithmsBase,
					  public IHack {
public:
	Md_5Algorithm();

	void PasswordToKey() override;
	void Encrypt(const std::string& filePathDest, const std::string& filePathSrc) override;
	void Decrypt(const std::string& filePathDest, const std::string& filePathSrc) override;

	bool SearchPassword(std::string const& fileSrc) override;

	size_t GetIndex() const;

	// Inherited via IHack
	virtual void AddToVocab(char from, char to) override;
	virtual void AddToVocab(std::string const& str) override;
	virtual void ResetVocab() override;
	virtual void SetMaxLenOfPassword(size_t sz) override;
	virtual size_t GetAmount() const override;

protected:
	void CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash) override;
	void EncryptAes(const std::vector<unsigned char> & plainText, std::vector<unsigned char>& chipherText) override;
	void DecryptAes(const std::vector<unsigned char> & chipherText, std::vector<unsigned char>& decryptText) override;
	
	bool CheckPass(const std::vector<unsigned char>& chipherText) override;
	void PasswordToKey(std::string& pass);
	void GetHash(std::vector<unsigned char>& hash, std::string const& fileSrc) const;
	void GetHash(std::vector<unsigned char>& dest, std::vector<unsigned char> const& src) const;

private:
	const EVP_MD* m_dgst;
	PasswordGenerator m_generator;
};

