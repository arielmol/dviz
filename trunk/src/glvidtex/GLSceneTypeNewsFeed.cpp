#include "GLSceneTypeNewsFeed.h"
#include "GLImageDrawable.h"
#include "qrencode-3.1.0/qrencode.h"

GLSceneTypeNewsFeed::GLSceneTypeNewsFeed(QObject *parent)
	: GLSceneType(parent)
	, m_currentIndex(0)
{
	m_fieldInfoList 
		<< FieldInfo("title", 
			"News Item Title", 
			"Title of the current news item", 
			"Text", 
			true)
			
		<< FieldInfo("text", 
			"Text snippet of the current news item.", 
			"A short four-six line summary of the current news item.", 
			"Text", 
			true)
			
		<< FieldInfo("source", 
			"Name of the news source", 
			"A short string giving the source of the news item.", 
			"Text", 
			false)
			
		<< FieldInfo("qrcode", 
			"QR Code", 
			"An optional image QR Code with a link to the news item.", 
			"Image", 
			false)
		
		<< FieldInfo("date", 
			"Date", 
			"A string giving the date of the item, such as '13 minutes ago'", 
			"Text", 
			false)
		;
		
	m_paramInfoList
		<< ParameterInfo("updateTime",
			"Update Time",
			"Time in minutes to wait between updates",
			QVariant::Int,
			true,
			SLOT(setUpdateTime(int)));
			
	PropertyEditorFactory::PropertyEditorOptions opts;
	
	opts.reset();
	opts.min = 1;
	opts.max = 30;
	m_paramInfoList[0].hints = opts;
	
	connect(&m_reloadTimer, SIGNAL(timeout()), this, SLOT(reloadData()));
	setParam("updateTime", 15);
	
	reloadData();
}

void GLSceneTypeNewsFeed::setLiveStatus(bool flag)
{
	GLSceneType::setLiveStatus(flag);
	
// 	if(m_news.size() > 0)
// 		showNextItem();
	
	if(flag)
	{
		m_reloadTimer.start();
		applyFieldData();
	}
	else
	{
		m_reloadTimer.stop();
		QTimer::singleShot( 0, this, SLOT(showNextItem()) );
	}
}

void GLSceneTypeNewsFeed::showNextItem()
{
	if(m_currentIndex < 0 || m_currentIndex >= m_news.size())
		m_currentIndex = 0;
		
	NewsItem item = m_news[m_currentIndex];
	
	setField("title", 	item.title);
	setField("text", 	item.text);
	setField("source",	item.source);
	setField("date",	item.date);
	
	GLDrawable *qrdest = lookupField("qrcode");
	if(qrdest)
	{
		QRcode* rawcode = QRcode_encodeString(qPrintable(item.url), 0, QR_ECLEVEL_M, QR_MODE_AN,  0);
		if(!rawcode)
			qDebug() << "GLSceneTypeNewsFeed::showNextItem(): Error generating qrcode.";
		else
		{
			QImage image(rawcode->width*4,rawcode->width*4,QImage::Format_ARGB32);
			memset(image.scanLine(0),0,image.byteCount());
			
			QPainter painter(&image);
			painter.setBrush(Qt::black);
			int w = rawcode->width;
			for(int x=0;x<w;x++)
			{
				for(int y=0;y<w;y++)
				{
					uchar data = rawcode->data[y*w+x];
					bool hasDot = data & 0xFF;
					if(hasDot)
					{
						painter.drawRect(QRect(x*4,y*4,4,4));
					}
				}
			}
			
			
			painter.end();
			
			dynamic_cast<GLImageDrawable*>(qrdest)->setImage(image);
		}
	}
	
	m_currentIndex++;
}

void GLSceneTypeNewsFeed::setParam(QString param, QVariant value)
{
	GLSceneType::setParam(param, value);
	
	if(param == "updateTime")
		m_reloadTimer.setInterval(value.toInt() * 60 * 1000);
}

void GLSceneTypeNewsFeed::reloadData()
{
	requestData("");//location());
}

void GLSceneTypeNewsFeed::requestData(const QString &location) 
{
	QUrl url("http://www.google.com/ig/api?news");
// 	url.addEncodedQueryItem("hl", "en");
// 	url.addEncodedQueryItem("weather", QUrl::toPercentEncoding(location));
	
	//qDebug() << "GLSceneTypeNewsFeed::requestData("<<location<<"): url:"<<url;

	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(handleNetworkData(QNetworkReply*)));
	manager->get(QNetworkRequest(url));
}

void GLSceneTypeNewsFeed::handleNetworkData(QNetworkReply *networkReply) 
{
	QUrl url = networkReply->url();
	if (!networkReply->error())
		parseData(QString::fromUtf8(networkReply->readAll()));
	networkReply->deleteLater();
	networkReply->manager()->deleteLater();
}

#define GET_DATA_ATTR xml.attributes().value("data").toString() \
	.replace("&amp;#39;","'") \
	.replace("&amp;quot;","\"")
	

void GLSceneTypeNewsFeed::parseData(const QString &data) 
{
	//qDebug() << "GLSceneTypeNewsFeed::parseData()";
	m_news.clear();
	m_currentIndex = 0;

	QXmlStreamReader xml(data);
	while (!xml.atEnd()) 
	{
		xml.readNext();
		if (xml.tokenType() == QXmlStreamReader::StartElement) 
		{
			// Parse and collect the news items
			if (xml.name() == "news_entry") 
			{
				NewsItem item;
				while (!xml.atEnd()) 
				{
					xml.readNext();
					if (xml.name() == "news_entry") 
					{
						if (!item.title.isEmpty()) 
						{
							m_news << item;
							//qDebug() << "GLSceneTypeNewsFeed::parseData(): Added item: "<<item.title;
							item = NewsItem();
						} 
						break;
					}
					
					if (xml.tokenType() == QXmlStreamReader::StartElement) 
					{
						if (xml.name() == "title")
							item.title = GET_DATA_ATTR;
						if (xml.name() == "url") 
							item.url = GET_DATA_ATTR;
						if (xml.name() == "source") 
							item.source = GET_DATA_ATTR;
						if (xml.name() == "date") 
							item.date = GET_DATA_ATTR;
						if (xml.name() == "snippet")
							item.text = GET_DATA_ATTR;
					}
				}
			}
		}
	}
	
	if(scene())
		showNextItem();
}
