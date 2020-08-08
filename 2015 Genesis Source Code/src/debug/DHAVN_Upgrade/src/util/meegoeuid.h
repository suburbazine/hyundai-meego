#ifndef	MEEGO_EUID_H_
#define	MEEGO_EUID_H_

class MeegoEUID{
public:
	MeegoEUID();
	~MeegoEUID();

	bool success() { return m_successToChange; }

private:
	bool m_successToChange;
};

#endif
