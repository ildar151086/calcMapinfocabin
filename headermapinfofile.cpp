#include "headermapinfofile.h"

HeaderMapinfoFile::HeaderMapinfoFile()
{

}

QList<QString> HeaderMapinfoFile::getHeaderPointForMap()
{
    QList<QString> headerPointForMap;

    headerPointForMap.append("Version   450");
    headerPointForMap.append("Charset \"WindowsCyrillic\"");
    headerPointForMap.append("Delimiter \",\"");
    headerPointForMap.append("CoordSys Earth Projection 8, 1001, \"m\", 45, 0, 1, 8500000, 0 Bounds (250718.46099, -10002137.4978) (16749281.539, 10002137.4978)");
    headerPointForMap.append("Columns 2");
    headerPointForMap.append("  A Char(9)");
    headerPointForMap.append("  B Char(9)");
    headerPointForMap.append("Data");

    return headerPointForMap;
}
QList<QString> HeaderMapinfoFile::getHeaderLineForMap()
{
    QList<QString> headerLineForMap;

    headerLineForMap.append("Version   450");
    headerLineForMap.append("Charset \"WindowsCyrillic\"");
    headerLineForMap.append("Delimiter \",\"");
    headerLineForMap.append("CoordSys Earth Projection 8, 1001, \"m\", 45, 0, 1, 8500000, 0 Bounds (250718.46099, -10002137.4978) (16749281.539, 10002137.4978)");
    headerLineForMap.append("Columns 1");
    headerLineForMap.append(" ЛинииR Char(10)");
    headerLineForMap.append("  data");

    return headerLineForMap;
}

QList<QString> HeaderMapinfoFile::getHeaderAreaForMap()
{
    QList<QString> headerAreaForMap;

    headerAreaForMap.append("Version   450");
    headerAreaForMap.append("Charset \"WindowsCyrillic\"");
    headerAreaForMap.append("Delimiter \",\"");
    headerAreaForMap.append("CoordSys Earth Projection 8, 1001, \"m\", 45, 0, 1, 8500000, 0 Bounds (250718.46099, -10002137.4978) (16749281.539, 10002137.4978)");
    headerAreaForMap.append("Columns 1");
    headerAreaForMap.append(" Площадки_в_узлах Char(10)");
    headerAreaForMap.append("  data");

    return headerAreaForMap;
}
