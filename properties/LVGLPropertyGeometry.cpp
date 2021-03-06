#include "LVGLPropertyGeometry.h"

#include <QSpinBox>
#include <QJsonObject>
#include <QDebug>
#include "LVGLObject.h"

class LVGLPropertyX : public LVGLPropertyCoord
{
public:
	inline LVGLPropertyX(LVGLProperty *p) : LVGLPropertyCoord(Qt::Horizontal, p) {}
	inline QString name() const override { return "x"; }

protected:
	inline lv_coord_t get(LVGLObject *obj) const override { return lv_obj_get_x(obj->obj()); }
	inline void set(LVGLObject *obj, lv_coord_t value) override { obj->setX(value); }

};

class LVGLPropertyY : public LVGLPropertyCoord
{
public:
	inline LVGLPropertyY(LVGLProperty *p) : LVGLPropertyCoord(Qt::Vertical, p) {}
	inline QString name() const override { return "y"; }

protected:
	inline lv_coord_t get(LVGLObject *obj) const override { return lv_obj_get_y(obj->obj()); }
	inline void set(LVGLObject *obj, lv_coord_t value) override { obj->setY(value); }

};

class LVGLPropertyWidth : public LVGLPropertyCoord
{
public:
	inline LVGLPropertyWidth(LVGLProperty *p) : LVGLPropertyCoord(Qt::Horizontal, p) {}
	inline QString name() const override { return "Width"; }

protected:
	inline lv_coord_t get(LVGLObject *obj) const override { return lv_obj_get_width(obj->obj()); }
	inline void set(LVGLObject *obj, lv_coord_t value) override { lv_obj_set_width(obj->obj(), value); }
};

class LVGLPropertyHeight : public LVGLPropertyCoord
{
public:
	inline LVGLPropertyHeight(LVGLProperty *p) : LVGLPropertyCoord(Qt::Vertical, p) {}
	inline QString name() const override { return "Height"; }

protected:
	inline lv_coord_t get(LVGLObject *obj) const override { return lv_obj_get_height(obj->obj()); }
	inline void set(LVGLObject *obj, lv_coord_t value) override { lv_obj_set_height(obj->obj(), value); }
};

LVGLPropertyGeometry::LVGLPropertyGeometry(LVGLProperty *parent)
	: LVGLProperty(parent)
	, m_x(new LVGLPropertyX(this))
	, m_y(new LVGLPropertyY(this))
	, m_w(new LVGLPropertyWidth(this))
	, m_h(new LVGLPropertyHeight(this))
{
	m_childs << m_x << m_y << m_w << m_h;
}

QString LVGLPropertyGeometry::name() const
{
	return "Geometry";
}

QVariant LVGLPropertyGeometry::value(LVGLObject *obj) const
{
	return QString("[(%1,%2) %3x%4]").arg(m_x->value(obj).toInt())
												.arg(m_y->value(obj).toInt())
												.arg(m_w->value(obj).toInt())
												.arg(m_h->value(obj).toInt());
}

void LVGLPropertyGeometry::setValue(LVGLObject *obj, QVariant value)
{
	if (value.type() == QVariant::Map) {
		QVariantMap map = value.toMap();
		obj->setGeometry(QRect(map["x"].toInt(), map["y"].toInt(),
									  map["width"].toInt(), map["height"].toInt()));
	}
}

QJsonValue LVGLPropertyGeometry::toJson(LVGLObject *obj) const
{
	return QJsonObject({{"x", m_x->value(obj).toInt()},
							  {"y", m_y->value(obj).toInt()},
							  {"width", m_w->value(obj).toInt()},
							  {"height", m_h->value(obj).toInt()}});
}

QStringList LVGLPropertyGeometry::function(LVGLObject *obj) const
{
	QStringList ret;
	ret << QString("lv_obj_set_pos(%1, %2, %3);").arg(obj->codeName()).arg(m_x->value(obj).toInt()).arg(m_y->value(obj).toInt());
	ret << QString("lv_obj_set_size(%1, %2, %3);").arg(obj->codeName()).arg(m_w->value(obj).toInt()).arg(m_h->value(obj).toInt());
	return ret;
}
