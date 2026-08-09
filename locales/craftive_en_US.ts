<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="en_US">
<context>
    <name>BasicProduct</name>
    <message>
        <location filename="../src/core/products.cpp" line="36"/>
        <source>From BasicProduct::fromFile(); failed to open the product file at %1 (%2)</source>
        <translation>from BasicProduct::fromFile(); failed to open the product file at %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="53"/>
        <source>From BasicProduct::toFile(); failed to open the product file at %1 (%2)</source>
        <translation>from BasicProduct::toFile(); failed to open the product file at %1 (%2)</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="133"/>
        <location filename="../src/core/products.cpp" line="283"/>
        <location filename="../src/core/products.hpp" line="12"/>
        <source>Empty</source>
        <translation>Empty</translation>
    </message>
</context>
<context>
    <name>Block</name>
    <message>
        <location filename="../src/core/elements.cpp" line="146"/>
        <source>From Block::fromJson(); the block&apos;s JSON isn&apos;t an object</source>
        <translation>from Block::fromJson(); the block&apos;s JSON isn&apos;t an object</translation>
    </message>
    <message>
        <location filename="../src/core/elements.cpp" line="153"/>
        <source>From Block::fromJson(); couldn&apos;t find &quot;id&quot; in the block&apos;s JSON</source>
        <translation>From Block::fromJson(); couldn&apos;t find &quot;id&quot; in the block&apos;s JSON</translation>
    </message>
    <message>
        <location filename="../src/core/elements.cpp" line="163"/>
        <source>From Block::fromJson(); couldn&apos;t find &quot;blk&quot; in the JSON of the block %1 (ID: %2)</source>
        <translation>From Block::fromJson(); couldn&apos;t find &quot;blk&quot; in the JSON of the block %1 (ID: %2)</translation>
    </message>
</context>
<context>
    <name>CoreStatus</name>
    <message>
        <location filename="../src/core/status.cpp" line="39"/>
        <source>From CoreStatus::getKit(); no one&apos;s ID is %1 in all loaded kits, please load this kit and try again</source>
        <translation>from CoreStatus::getKit(); no one&apos;s ID is %1 in all loaded kits, please load this kit and try again</translation>
    </message>
</context>
<context>
    <name>ErrDialog</name>
    <message>
        <location filename="../src/frontend/ErrDialog.qml" line="7"/>
        <location filename="../src/frontend/ErrDialog.qml" line="20"/>
        <source>Oops! Error %1 &quot;%2&quot; occurred.</source>
        <translation>Oops! Error %1 &quot;%2&quot; occurred.</translation>
    </message>
    <message>
        <location filename="../src/frontend/ErrDialog.qml" line="10"/>
        <source>Retry</source>
        <translation>Retry</translation>
    </message>
    <message>
        <location filename="../src/frontend/ErrDialog.qml" line="11"/>
        <source>Cancel</source>
        <translation>Cancel</translation>
    </message>
    <message>
        <location filename="../src/frontend/ErrDialog.qml" line="8"/>
        <location filename="../src/frontend/ErrDialog.qml" line="21"/>
        <source>Detail: %1</source>
        <translation>Detail: %1</translation>
    </message>
</context>
<context>
    <name>Kit</name>
    <message>
        <location filename="../src/core/products.cpp" line="86"/>
        <source>From Kit::operator-=; this kit doesn&apos;t contain an element who&apos;s ID is &quot;%1&quot;</source>
        <translation>from Kit::operator-=; this kit doesn&apos;t contain an element who&apos;s ID is &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="103"/>
        <source>From Kit::operator[]; this kit doesn&apos;t contain an element who&apos;s ID is &quot;%1&quot;</source>
        <translation>from Kit::operator[]; this kit doesn&apos;t contain an element who&apos;s ID is &quot;%1&quot;</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="110"/>
        <source>From Kit::fromJson(); the kit&apos;s JSON isn&apos;t an object</source>
        <translation>from Kit::fromJson(); the kit&apos;s JSON isn&apos;t an object</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="117"/>
        <source>From Kit::fromJson(); couldn&apos;t find &quot;id&quot; in the kit&apos;s JSON</source>
        <translation>from Kit::fromJson(); couldn&apos;t find &quot;id&quot; in the kit&apos;s JSON</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="128"/>
        <source>From Kit::fromJson(); couldn&apos;t find &quot;author&quot; in the JSON of Kit %1 (ID: %2)</source>
        <translation>from Kit::fromJson(); couldn&apos;t find &quot;author&quot; in the JSON of Kit %1 (ID: %2)</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="141"/>
        <source>From Kit::fromJson(); couldn&apos;t find &quot;ver&quot; in the JSON of Kit %1 (ID: %2)</source>
        <translation>from Kit::fromJson(); couldn&apos;t find &quot;ver&quot; in the JSON of Kit %1 (ID: %2)</translation>
    </message>
</context>
<context>
    <name>LBlock</name>
    <message>
        <location filename="../src/core/elements.cpp" line="219"/>
        <source>From LBlock::getPos(); the required position is (%1,%2), but it&apos;s out of range</source>
        <translation>from LBlock::getPos(); the required position is (%1,%2), but it&apos;s out of range</translation>
    </message>
    <message>
        <location filename="../src/core/elements.cpp" line="232"/>
        <source>From LBlock::setPos(); the required position is (%1,%2), but it&apos;s out of range</source>
        <translation>from LBlock::setPos(); the required position is (%1,%2), but it&apos;s out of range</translation>
    </message>
    <message numerus="yes">
        <location filename="../src/core/elements.cpp" line="250"/>
        <source>From LBlock::getW(); the required row number is %1, but there&apos;s only %n row(s) in this large-block</source>
        <translation>
            <numerusform>from LBlock::getW(); the required row number is %1, but there&apos;s only %n row in this large-block</numerusform>
            <numerusform>from LBlock::getW(); the required row number is %1, but there&apos;s only %n rows in this large-block</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/core/elements.cpp" line="267"/>
        <source>From LBlock::setW()</source>
        <translation>from LBlock::setW()</translation>
    </message>
    <message>
        <location filename="../src/core/elements.cpp" line="276"/>
        <source>From LBlock::fromJson(); the large-block&apos;s JSON isn&apos;t an object</source>
        <translation>from LBlock::fromJson(); the large-block&apos;s JSON isn&apos;t an object</translation>
    </message>
    <message>
        <location filename="../src/core/elements.cpp" line="284"/>
        <source>From LBlock::fromJson(); couldn&apos;t find &quot;id&quot; in the large-block&apos;s JSON</source>
        <translation>From LBlock::fromJson(); couldn&apos;t find &quot;id&quot; in the large-block&apos;s JSON</translation>
    </message>
    <message>
        <location filename="../src/core/elements.cpp" line="294"/>
        <source>From LBlock::fromJson(); the &quot;lblk&quot; isn&apos;t an array in the JSON of the large-block %1 (ID: %2)</source>
        <translation>From LBlock::fromJson(); the &quot;lblk&quot; isn&apos;t an array in the JSON of the large-block %1 (ID: %2)</translation>
    </message>
    <message>
        <location filename="../src/core/elements.cpp" line="303"/>
        <source>From LBlock::fromJson(); Row %1 isn&apos;t an array in the JSON of the large-block %1 (ID: %2)</source>
        <translation>From LBlock::fromJson(); Row %1 isn&apos;t an array in the JSON of the large-block %1 (ID: %2)</translation>
    </message>
    <message>
        <location filename="../src/core/elements.cpp" line="315"/>
        <source>From LBlock::fromJson(); couldn&apos;t find &quot;lblk&quot; in the JSON of the large-block %1 (ID: %2)</source>
        <translation>From LBlock::fromJson(); couldn&apos;t find &quot;lblk&quot; in the JSON of the large-block %1 (ID: %2)</translation>
    </message>
    <message>
        <location filename="../src/core/elements.cpp" line="324"/>
        <source>From LBlock::fromJson(); the &quot;w&quot; isn&apos;t an array in the JSON of the large-block %1 (ID: %2)</source>
        <translation>From LBlock::fromJson(); the &quot;w&quot; isn&apos;t an array in the JSON of the large-block %1 (ID: %2)</translation>
    </message>
</context>
<context>
    <name>Main</name>
    <message>
        <location filename="../src/frontend/Main.qml" line="43"/>
        <source>Home</source>
        <translation>Home</translation>
    </message>
    <message>
        <location filename="../src/frontend/Main.qml" line="47"/>
        <source>Store</source>
        <translation>Store</translation>
    </message>
    <message>
        <location filename="../src/frontend/Main.qml" line="51"/>
        <source>Play</source>
        <translation>Play</translation>
    </message>
    <message>
        <location filename="../src/frontend/Main.qml" line="55"/>
        <source>Craftboard</source>
        <translation>Craftboard</translation>
    </message>
    <message>
        <location filename="../src/frontend/Main.qml" line="59"/>
        <source>Myself</source>
        <translation>Myself</translation>
    </message>
    <message>
        <location filename="../src/frontend/Main.qml" line="63"/>
        <source>Settings</source>
        <translation>Settings</translation>
    </message>
</context>
<context>
    <name>Map</name>
    <message>
        <location filename="../src/core/products.cpp" line="188"/>
        <source>From Map::operator[]; the required position is (%1,%2), but it&apos;s out of range</source>
        <translation>from Map::operator[]; the required position is (%1,%2), but it&apos;s out of range</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="218"/>
        <source>From Map::get&lt;0&gt;(); the required position is (%1,%2), but it&apos;s out of range</source>
        <translation>from Map::get&lt;0&gt;(); the required position is (%1,%2), but it&apos;s out of range</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="234"/>
        <source>From Map::get&lt;1&gt;(); the required position is (%1,%2), but it&apos;s out of range</source>
        <translation>from Map::get&lt;1&gt;(); the required position is (%1,%2), but it&apos;s out of range</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="260"/>
        <source>From Map::fromJson(); the map&apos;s JSON isn&apos;t an object</source>
        <translation>from Map::fromJson(); the map&apos;s JSON isn&apos;t an object</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="267"/>
        <source>From Map::fromJson(); couldn&apos;t find &quot;id&quot; in the map&apos;s JSON</source>
        <translation>from Map::fromJson(); couldn&apos;t find &quot;id&quot; in the map&apos;s JSON</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="278"/>
        <source>From Map::fromJson(); couldn&apos;t find &quot;author&quot; in the JSON of Map %1 (ID: %2)</source>
        <translation>from Map::fromJson(); couldn&apos;t find &quot;author&quot; in the JSON of Map %1 (ID: %2)</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="291"/>
        <source>From Map::fromJson(); couldn&apos;t find &quot;ver&quot; in the JSON of Map %1 (ID: %2)</source>
        <translation>from Map::fromJson(); couldn&apos;t find &quot;ver&quot; in the JSON of Map %1 (ID: %2)</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="299"/>
        <source>From Map::fromJson(); couldn&apos;t find &quot;data&quot; in the JSON of Map %1 (ID: %2)</source>
        <translation>from Map::fromJson(); couldn&apos;t find &quot;data&quot; in the JSON of Map %1 (ID: %2)</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="305"/>
        <source>From Map::fromJson(); &quot;data&quot; in the JSON of Map %1 (ID: %2) isn&apos;t an array</source>
        <translation>from Map::fromJson(); &quot;data&quot; in the JSON of Map %1 (ID: %2) isn&apos;t an array</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="311"/>
        <source>From Map::fromJson(); Row %1 in &quot;data&quot; in the JSON of Map %2 (ID: %3) isn&apos;t an array</source>
        <translation>from Map::fromJson(); Row %1 in &quot;data&quot; in the JSON of Map %2 (ID: %3) isn&apos;t an array</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="320"/>
        <source>From Map::fromJson(); (%1,%2)&apos;s ID in &quot;data&quot; in the JSON of Map %3 (ID: %4) isn&apos;t a string</source>
        <translation>from Map::fromJson(); (%1,%2)&apos;s ID in &quot;data&quot; in the JSON of Map %3 (ID: %4) isn&apos;t a string</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="331"/>
        <source>From Map::fromJson(); couldn&apos;t find the kit (ID: %1) where contains the element at (%2,%3) (ID: %4) in the JSON of Map %5 (ID: %6), please load this kit and try again</source>
        <translation>from Map::fromJson(); couldn&apos;t find the kit (ID: %1) where contains the element at (%2,%3) (ID: %4) in the JSON of Map %5 (ID: %6), please load this kit and try again</translation>
    </message>
    <message>
        <location filename="../src/core/products.cpp" line="343"/>
        <source>From Map::fromJson(); Kit %1 (ID: %2) doesn&apos;t contain the element at (%3,%4) (ID: %5) in Map %6 (ID: %7)</source>
        <translation>from Map::fromJson(); Kit %1 (ID: %2) doesn&apos;t contain the element at (%3,%4) (ID: %5) in Map %6 (ID: %7)</translation>
    </message>
</context>
<context>
    <name>Settings</name>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="22"/>
        <source>Settings</source>
        <translation>Settings</translation>
    </message>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="33"/>
        <source>Appearance</source>
        <translation>Appearance</translation>
    </message>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="58"/>
        <source>Theme</source>
        <translation>Theme</translation>
    </message>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="63"/>
        <source>Using dark theme now</source>
        <translation>Using dark theme now</translation>
    </message>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="63"/>
        <source>Using light theme now</source>
        <translation>Using light theme now</translation>
    </message>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="102"/>
        <source>Theme Color</source>
        <translation>Theme Color</translation>
    </message>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="107"/>
        <source>Select a seed color to generate the Material You color scheme</source>
        <translation>Select a seed color to generate the Material You color scheme</translation>
    </message>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="189"/>
        <source>Or select more precisely below:</source>
        <translation>Or select more precisely below:</translation>
    </message>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="193"/>
        <source>Hue</source>
        <translation>Hue</translation>
    </message>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="194"/>
        <source>Chroma</source>
        <translation>Chroma</translation>
    </message>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="195"/>
        <source>Tone</source>
        <translation>Tone</translation>
    </message>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="207"/>
        <source>About</source>
        <translation>About</translation>
    </message>
    <message>
        <location filename="../src/frontend/pages/Settings.qml" line="231"/>
        <source>Version</source>
        <translation>Version</translation>
    </message>
</context>
<context>
    <name>Ucc</name>
    <message>
        <location filename="../src/core/elements.cpp" line="55"/>
        <source>From Ucc::fromJson(); the unicode colored character&apos;s JSON isn&apos;t an object</source>
        <translation>from Ucc::fromJson(); the unicode colored character&apos;s JSON isn&apos;t an object</translation>
    </message>
    <message>
        <location filename="../src/core/elements.cpp" line="59"/>
        <source>From Ucc::fromJson(); the string to parse is %1 and it has too many characters</source>
        <translation>from Ucc::fromJson(); the string to parse is %1 and it has too many characters</translation>
    </message>
</context>
<context>
    <name>Version</name>
    <message>
        <location filename="../src/core/crtutils.cpp" line="35"/>
        <source>From Version::fromJson(); the version&apos;s JSON isn&apos;t an array. It should be like [x, y, z] for &quot;x.y.z&quot;</source>
        <translation>from Version::fromJson(); the version&apos;s JSON isn&apos;t an array. It should be like [x, y, z] for &quot;x.y.z&quot;</translation>
    </message>
</context>
<context>
    <name>checkElemID</name>
    <message>
        <location filename="../src/core/crtutils.cpp" line="100"/>
        <source>From checkElemID(); the ID isn&apos;t like &quot;kit/elem&quot;</source>
        <translation>From checkElemID(); the ID isn&apos;t like &quot;kit/elem&quot;</translation>
    </message>
</context>
<context>
    <name>checkEmail</name>
    <message>
        <location filename="../src/core/crtutils.cpp" line="115"/>
        <source>From checkEmail(); the string is &quot;%1&quot; and it isn&apos;t a valid email address</source>
        <translation>From checkEmail(); the string is &quot;%1&quot; and it isn&apos;t a valid email address</translation>
    </message>
</context>
<context>
    <name>checkID</name>
    <message>
        <location filename="../src/core/crtutils.cpp" line="85"/>
        <location filename="../src/core/crtutils.cpp" line="88"/>
        <source>From checkID()</source>
        <translation>From checkID()</translation>
    </message>
</context>
<context>
    <name>errmsgs</name>
    <message>
        <location filename="../src/core/err.cpp" line="6"/>
        <source>Extern error</source>
        <translation>Extern error</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="7"/>
        <source>Empty ID</source>
        <translation>Empty ID</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="8"/>
        <source>Invalid ID</source>
        <translation>Invalid ID</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="9"/>
        <source>Invalid unicode colored character</source>
        <translation>Invalid unicode colored character</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="10"/>
        <source>No such element in this kit</source>
        <translation>No such element in this kit</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="11"/>
        <source>Invalid location in this large-block</source>
        <translation>Invalid location in this large-block</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="12"/>
        <source>Failed to parse JSON</source>
        <translation>Failed to parse JSON</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="13"/>
        <source>Invalid email address</source>
        <translation>Invalid email address</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="14"/>
        <source>Invalid RGB color</source>
        <translation>Invalid RGB color</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="15"/>
        <source>Author doesn&apos;t exist</source>
        <translation>Author doesn&apos;t exist</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="16"/>
        <source>Data doesn&apos;t exist</source>
        <translation>Data doesn&apos;t exist</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="17"/>
        <source>No such kit in all loaded kits</source>
        <translation>No such kit in all loaded kits</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="18"/>
        <source>Invalid Element ID; it should be like &quot;kit/elem&quot;</source>
        <translation>Invalid Element ID; it should be like &quot;kit/elem&quot;</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="19"/>
        <source>Product version doesn&apos;t exist</source>
        <translation>Product version doesn&apos;t exist</translation>
    </message>
    <message>
        <location filename="../src/core/err.cpp" line="20"/>
        <source>Invalid location in this map</source>
        <translation>Invalid location in this map</translation>
    </message>
    <message>
        <location filename="../src/core/err.hpp" line="80"/>
        <location filename="../src/core/err.hpp" line="105"/>
        <source>From %1; the original error message is &quot;%2&quot;</source>
        <translation>From %1; the original error message is &quot;%2&quot;</translation>
    </message>
    <message>
        <location filename="../src/core/err.hpp" line="90"/>
        <location filename="../src/core/err.hpp" line="114"/>
        <source>From %1; unknown exception type, neither CrtExcept nor std::exception</source>
        <translation>From %1; unknown exception type, neither CrtExcept nor std::exception</translation>
    </message>
</context>
<context>
    <name>rgb</name>
    <message>
        <location filename="../src/core/elements.cpp" line="16"/>
        <source>From rgb::fromJson(); the RGB color in the JSON object isn&apos;t an array</source>
        <translation>from rgb::fromJson(); the RGB color in the JSON object isn&apos;t an array</translation>
    </message>
    <message>
        <location filename="../src/core/elements.cpp" line="18"/>
        <source>From rgb::fromJson(); incorrect number of color channels</source>
        <translation>from rgb::fromJson(); incorrect number of color channels</translation>
    </message>
</context>
<context>
    <name>separateElemID</name>
    <message>
        <location filename="../src/core/crtutils.cpp" line="150"/>
        <source>From separateElemID(); the ID is %1</source>
        <translation>From separateElemID(); the ID is %1</translation>
    </message>
</context>
</TS>
