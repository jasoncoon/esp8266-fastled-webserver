typedef struct {
  long utcOffsetInSeconds;
  String utcOffset;
  String region;
} TimeZone;
typedef TimeZone TimeZoneList[];

const TimeZoneList timeZones = {
  {-43200, "UTC−12:00", "Baker Island, Howland Island (both uninhabited)"},
  {-39600, "UTC−11:00", "American Samoa, Niue"},
  {-36000, "UTC−10:00", "Honolulu"},
  {-34200, "UTC−09:30", "Marquesas Islands"},
  {-32400, "UTC−09:00", "Anchorage"},
  {-28800, "UTC−08:00", "Los Angeles, Vancouver, Tijuana"},
  {-25200, "UTC−07:00", "Phoenix, Calgary, Ciudad Juárez"},
  {-21600, "UTC−06:00", "Mexico City, Chicago, Guatemala City, Tegucigalpa, Winnipeg, San José, San Salvador"},
  {-18000, "UTC−05:00", "New York, Toronto, Havana, Lima, Bogotá, Kingston"},
  {-14400, "UTC−04:00", "Santiago, Santo Domingo, Manaus, Caracas, La Paz, Halifax"},
  {-12600, "UTC−03:30", "St. John’s"},
  {-10800, "UTC−03:00", "São Paulo, Buenos Aires, Montevideo"},
  { -7200, "UTC−02:00", "Brazil (Fernando de Noronha), South Georgia and the South Sandwich Islands"},
  { -3600, "UTC−01:00", "Cape Verde"},
  {     0, "UTC±00:00", "London, Dublin, Lisbon, Abidjan, Accra, Dakar"},
  {  3600, "UTC+01:00", "Berlin, Rome, Paris, Madrid, Warsaw, Lagos, Kinshasa, Algiers, Casablanca"},
  {  7200, "UTC+02:00", "Cairo, Johannesburg, Khartoum, Kiev, Bucharest, Athens, Jerusalem, Sofia"},
  { 10800, "UTC+03:00", "Moscow, Istanbul, Riyadh, Baghdad, Addis Ababa, Doha"},
  { 12600, "UTC+03:30", "Tehran"},
  { 14400, "UTC+04:00", "Dubai, Baku, Tbilisi, Yerevan, Samara"},
  { 16200, "UTC+04:30", "Kabul"},
  { 18000, "UTC+05:00", "Karachi, Tashkent, Yekaterinburg"},
  { 19800, "UTC+05:30", "Mumbai, Colombo"},
  { 20700, "UTC+05:45", "Kathmandu"},
  { 21600, "UTC+06:00", "Dhaka, Almaty, Omsk"},
  { 23400, "UTC+06:30", "Yangon"},
  { 25200, "UTC+07:00", "Jakarta, Ho Chi Minh City, Bangkok, Krasnoyarsk"},
  { 28800, "UTC+08:00", "Shanghai, Taipei, Kuala Lumpur, Singapore, Perth, Manila, Makassar, Irkutsk"},
  { 31500, "UTC+08:45", "Australia (Eucla) unofficial"},
  { 32400, "UTC+09:00", "Tokyo, Seoul, Pyongyang, Ambon, Yakutsk"},
  { 34200, "UTC+09:30", "Adelaide"},
  { 36000, "UTC+10:00", "Sydney, Port Moresby, Vladivostok"},
  { 37800, "UTC+10:30", "Lord Howe Island"},
  { 39600, "UTC+11:00", "Nouméa, Magadan"},
  { 43200, "UTC+12:00", "Auckland, Suva, Petropavlovsk-Kamchatsky"},
  { 45900, "UTC+12:45", "Chatham Islands"},
  { 46800, "UTC+13:00", "Kiribati (Phoenix Islands), Tonga, Tokelau"},
  { 50400, "UTC+14:00", "Kiribati (Line Islands)"}
};

const uint8_t timeZoneCount = ARRAY_SIZE(timeZones);
