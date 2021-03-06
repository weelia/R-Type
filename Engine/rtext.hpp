#ifndef RTEXT_HPP
#define RTEXT_HPP

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "ressource.hpp"

class RText : public sf::Text, public Ressource
{
public:
	typedef bool (RText::*action)(QMap<QString, QString>&, const sf::Input&);

	RText(QString name, QString text);

	static QMap<QString, action> actions;
protected:
	bool do_display(QMap<QString, QString>& properties, const sf::Input& input);
	bool do_transition(QMap<QString, QString>& properties, const sf::Input& input);

	void initActions();

	QMap<QString, QString> _properties;
	QString _text;
	bool first;
};

#endif // RTEXT_HPP
