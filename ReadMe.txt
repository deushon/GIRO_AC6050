RU/EN
RU
Данная библиотека позволяет взаимодействовать с 6050 MPU сенсором со встроенным STM контролером через интерфейс UART.
Представлены реализации библиотеки для различных ЯП и платформ
	Реализовано для:
		Arduino (C++);
	В планах:
		Widows/Linux:
			C++;
			C#;
			Python.
Инициализация датчика и билиотеки будут отличаться в зависимости от платформы, но общая структура такова:
GIRO_AC6050 предоставляет класс для получения данных, их хранения.
		Каждая функция обновляет свой блок данных. 
		Всего существует 4 блока Ускорение Ac; Угловая скорость Av; Текущий угол An (сбрасывается при перезагрузке датчика); Температура T.
		Можно получать только RAW данные или обработанные значения GetX или GetXRAW.
		Все функции:
			Чтение RAW:
			GetAcRAW(); GetAvRAW(); GetAnRAW(); GetTRAW();
			Чтение расчетных значений:
			GetAc(); GetAv(); GetAn(); GetT();
		Функции возвращают true - если данные успешно получены и обновлены. false - в случае ошибки чтения или таймаута соединения.
		Успешный вызов функции обновляет значения в соответствующем блоке.
		double rawXac, rawYac, rawZac, rawXav, rawYav, rawZav, rawXan, rawYan, rawZan, rawTEMPER;
		Или
		double Xac, Yac, Zac, Xav, Yav, Zav, Xan, Yan, Zan, TEMPER;
		Соответственно доступ к ним можно получить через ваш экземпляр класса, например AXER.TEMPER;
		Если обновить значение не удалось, значения будут равны либо = 0 либо предыдущему полученному.


EN
This library provide work with MPU6050 STM32 Kalman Serial IMU module by UART.


You can learn more in libraries examples