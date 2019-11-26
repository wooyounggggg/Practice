#include "ConsoleView.h"

ConsoleView::ConsoleView(Data* data, FileManager* fm) { // ������ / data�� view�� ����
	this->data = data;
	this->option = '\0'; // �ʱ�ȭ
	this->fm = fm;
	this->comboAndSet = '\0'; // �ʱ�ȭ
}

void ConsoleView::gotoxy(short x, short y) { // Ű���� Ŀ���� x,y ��ǥ�� �̵���Ű�� �޼ҵ�
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void ConsoleView::showUI() { // �޴� ����Ŭ ���
	bool exit = false; // Ű����ũ ���� ����
	while (1) {
		switch (page) {
		case 0: // �ʱ�ȭ�� & ī�װ� ���� & īƮ ���� & ����
			page = userUI0();
			break;
		case 1: // ��ǰ ����
			page = userUI1();
			break;
		case 2: // ���� ���� (��ǰ, �޺�, ��Ʈ)
			page = userUI2();
			break;
		case 3: // ��� ����
			page = userUI3();
			break;
		case 4: // ���̵�-���� ���� & īƮ ���
			page = userUI4();
			break;
		case 5: // īƮ Ȯ��
			page = userUI5();
			break;
		case 6: // ����-���� & ���� ����
			page = userUI6();
			break;
		case 7: // ������ ��� �ʱ�ȭ��
			page = adminUI0();
			break;
		case 8: // ����ǰ �߰�
			page = adminUI1();
			break;
		case 9: // ���� ��ǰ ����
			page = adminUI2();
			break;
		case 10: // ��� �Է�
			page = adminUI3();
			break;
		case 11: // ��� �ڷ� ��ȸ
				page = adminUI4();
				break;
		case 12: // ����
				page = helpUI();
				break;
		case -1: // ����
			exit = true;
			break;
		}
		if (exit == true) break;
	}
	cout << "Ű����ũ�� ����Ǿ����ϴ�.\n\n" << endl;
}

int ConsoleView::userUI0() { // �ʱ�ȭ�� & ī�װ� ���� & īƮ ���� & ����
	while (1) {
		cout << "h. ��뼳��\n" << endl;
		cout << "�� ī�װ� ��" << endl;
		cout << "  a. �ܹ���" << endl;
		cout << "  b. ���̵�" << endl;
		cout << "  c. ����Ʈ" << endl;
		cout << "  d. ����\n" << endl;

		// ���� īƮ ���
		border_above(); // �� �׸���

		cout << "�� < īƮ ��� >" << endl;
		gotoxy(101, 9); // Ŀ�� �̵�
		cout << "��" << endl;

		vector<vector<Product>> cartList = data->getCartList();
		for (unsigned int i = 0; i < cartList.size(); i++)
		{
			cout << "�� " << i + 1 << ". ";
			print_all_elements(cartList.at(i)); // cartList�� �ִ� ��� ��ҵ��� ���
			gotoxy(101, 10 + i);
			cout << "�� " << endl;
		}

		border_bottom(); // �� �׸���

		pay(); // ���� ��ư �׸���(UI)

		cout << " * ������ ���(@)" << endl;

		// ������ �ڵ� : �Ǹ��� īƮ ��ȸ 
		if (data->getSoldCartList().size() != 0) {
			cout << "-----------------------������ ���----------------------" << endl;
			for (size_t i = 0; i < data->getSoldCartList().size(); i++)
			{
				cout << "< Cart " << i << " >" << endl;
				print_all_elements(data->getSoldCartList().at(i));
				cout << endl;
			}
			cout << "���� ����� : " << data->get_total_sales() << "��" << endl;
			cout << "-----------------------������ ���----------------------" << endl;
		}
		// ������ �ڵ� : �Ǹ��� īƮ ��ȸ 

		cout << "\n - �Է� : ";
		inputs(); // Ű���� �Է� �ޱ�

		console_clear(); // �ܼ� ����

		if (input == 'h') return 12; // page 12�� �̵�

		if ((input >= 'a' && input <= 'd')) { // ī�װ� ���� ��
			if (input == 'a') burger = true; // page 2�� page 4 ������ �б⸦ ����
			else burger = false; // page 2�� page 4 ������ �б⸦ ����
			option = input; // ī�װ� ������ option�� ����
			return 1; // page 1�� �̵�
		}
		if (input >= '1' && input <= '1' + cartList.size() - 1) {
			data->getCartList().erase(data->getCartList().begin() + (input - 49)); // īƮ ���� (īƮ ����Ʈ�� ���ۿ��� i-1��° ������ �� ����) 
		}
		if (input == '$') return 5; // page 5�� �̵�
		if (input == '@') return 7; // page 7�� �̵�
	}
}

/*
 �� getCategoryArray()[0] = burgerCategory
 �� getCategoryArray()[1] = sideCategory
 �� getCategoryArray()[2] = dessertCategory
 �� getCategoryArray()[3] = drinkCategory
 �� getCategoryArray()[4] = addmaterialCategory
*/

int ConsoleView::userUI1() { // ��ǰ ����
	while (1) {
		cout << "* �ڷΰ���(0)\n" << endl;
		cout << "�� ��ǰ ��� ��" << endl;

		vector<Product>* category = data->getCategoryArray()[0]; // '�ƹ��ų�'�� �ʱ�ȭ �� 
		switch (option) { // option : ������ ������ ī�װ�
		case 'a': // �ܹ���
			category = data->getCategoryArray()[0];
			break;
		case 'b': // ���̵�
			category = data->getCategoryArray()[1];
			break;
		case 'c': // ����Ʈ
			category = data->getCategoryArray()[2];
			break;
		case 'd': // ����
			category = data->getCategoryArray()[3];
			break;
		}

		for (unsigned int i = 0; i < category->size(); i++) // page 0���� ������ option(ī�װ�)�� �ش� �Ǵ� ��ǰ ��� ��� 
		{
			cout << "  " << (char)(i + 97) << ". " << category->at(i).getName(); // (i + 97) �� a,b,c ... �� �ǹ�
			gotoxy(22, 3 + i); // ��������
			cout << category->at(i).getKcal() << "Kcal"; // Kcal���
			gotoxy(32, 3 + i); // ��������
			cout << category->at(i).getPrice() << "��" << endl; // Price��� 
		}

		cout << "\n - �Է� : ";
		inputs();
		console_clear();

		if (input >= 'a' && input <= 'a' + category->size() - 1) { // ��ǰ�� ������ ���
			data->getCurrentCart().push_back(data->getCategoryArray()[option - 'a']->at(input - 97));
			// �� īƮ�� ��ǰ ���� : [option - 'a']�� (0 ~ 4) ������ ������ ī�װ��� �ǹ�
			//						 (input - 97)�� ��� ������ ��ǰ�� ī�װ� �󿡼��� �ε����� �ǹ�

			if (burger == true) return 2; // ���Ÿ� ������ ���, page 2�� �̵�
			else {
				incresedCartSize = 1;
				return 4;
			} // ���Ű� �ƴ� ���, page 4�� �̵�
		}
		else if (input == '0')
			return 0;
		// �ڷ� ����
	}
}

int ConsoleView::userUI2() { // ���� ����
	while (1) {
		cout << "* �ڷΰ���(0)\n" << endl;
		cout << "[ īƮ�� ��� ��� : ";
		print_all_elements(data->getCurrentCart()); // ���� īƮ ��� ��� 
		cout << " ]\n" << endl;
		cout << "�� ���� ���� ��" << endl;

		cout << "  a. ��ǰ (����)" << endl;
		cout << "  b. �޺� (���� + ����)" << endl;
		cout << "  c. ��Ʈ (���� + ���̵� + ����)" << endl;

		cout << "\n - �Է� : ";
		inputs();
		comboAndSet = input;
		console_clear();

		if (input >= 'a' && input <= 'c') { // ���� ���� ��
			if (input == 'a') {
				side = false;
				drink = false;
			}
			else if (input == 'b') {
				incresedCartSize = 1; // īƮ 1 �þ
				data->getCurrentCart().push_back(data->getCategoryArray()[3]->at(0)); // �ݶ� �ڵ� ���� 
				side = false;
				drink = true;
			}
			else if (input == 'c') {
				incresedCartSize = 2; // īƮ 2 �þ
				data->getCurrentCart().push_back(data->getCategoryArray()[1]->at(0)); // ����Ƣ�� �ڵ� ���� 
				data->getCurrentCart().push_back(data->getCategoryArray()[3]->at(0)); // �ݶ� �ڵ� ���� 
				side = true;
				drink = true;
			}
			return 3; // page 3���� �̵�
		}
		else if (input == '0') {
			data->getCurrentCart().pop_back(); // �ڷ� ���� �� īƮ ��� 
			side = false;
			drink = false;
			return 1; // �ڷ� ����
		}
	}
}

int ConsoleView::userUI3() { // ��� ����
	while (1) {
		cout << "* �ڷΰ���(0)\n" << endl;
		cout << "[ īƮ�� ��� ��� : ";
		print_all_elements(data->getCurrentCart()); // ���� īƮ ��� ��� 
		cout << " ]\n" << endl;
		cout << "�� �߰� ��� ���� ��" << endl;

		for (unsigned int i = 0; i < data->getCategoryArray()[4]->size(); i++) // �߰���� ��� ��� 
		{
			cout << "  " << (char)(i + 97) << ". " << data->getCategoryArray()[4]->at(i).getName(); // (i + 97) �� a,b,c ... �� �ǹ�
			gotoxy(22, 5 + i); // ��������
			cout << data->getCategoryArray()[4]->at(i).getKcal() << "Kcal"; // Kcal���
			gotoxy(32, 5 + i); // ��������
			cout << data->getCategoryArray()[4]->at(i).getPrice() << "��" << endl; // Price��� 
		}
		cout << "  * �߰� �� �� : n" << endl;

		cout << "\n - �Է� : ";
		inputs();
		console_clear();
		if (input >= 'a' && input <= 'd' || input == 'n') { // ��� ���� ��
			if (input != 'n') {
				data->getCurrentCart().push_back(data->getCategoryArray()[4]->at(input - 'a')); // n�� �ƴ� ����, �߰� ��� ���� 
				incresedCartSize = 1; // īƮ 1 �þ 
			}
			else incresedCartSize = 0; // īƮ �ȴþ 
			return 4; // page 4�� �̵�
		}
		else if (input == '0') {
			while (incresedCartSize != 0) {
				data->getCurrentCart().pop_back();
				incresedCartSize--;
			} // �þ īƮ�� ����� 0�� �ɶ����� īƮ �Ѱ��� ��� 
			return 2; // �ڷ� ����
		}
	}
}

int ConsoleView::userUI4() { // ��ǰ ���� & īƮ ���
	while (true) {
		cout << "* �ڷΰ���(0)\n" << endl;
		cout << "[ īƮ�� ��� ��� : ";
		print_all_elements(data->getCurrentCart()); // ���� īƮ ��� ��� 
		cout << " ]\n" << endl;
		if (burger == true && (side == true || drink == true)) { // ���� �����̸鼭 ���ÿ�, ���̵� �Ǵ� ���� ���� �ÿ��� ���
			cout << "�� ��ǰ ���� ��" << endl;
			if (side == false) cout << "  a. ���� ����" << endl;
			else {
				cout << "  a. ���̵� ����" << endl;
				cout << "  b. ���� ����\n" << endl;
			}
		}
		cout << " * īƮ�� ���(y)" << endl;

		cout << "\n - �Է� : ";
		inputs();

		if (input == 'y') { // īƮ ���
			vector<Product> copiedCart;// ���� īƮ�� �״�� �����س��� īƮ
			copiedCart.assign(data->getCurrentCart().begin(), data->getCurrentCart().end()); // ���� 
			data->getCartList().push_back(copiedCart); // ���纻�� "��� īƮ ���"�� �߰� 
			data->getCurrentCart().clear(); // ���� īƮ ���� (����) 
			console_clear();
			return 0;
		}
		else if (input == '0') { // �ڷ� ����
			while (incresedCartSize != 0) {
				data->getCurrentCart().pop_back();
				incresedCartSize--;
			} // �þ īƮ�� ����� 0�� �ɶ����� īƮ �Ѱ��� ��� 
			console_clear();
			incresedCartSize = data->getCurrentCart().size() - 1; // īƮ����� ������ �þ īƮ�� ������� ���� 
			if (burger == true) return 3; // ���Ÿ� ������ ���, page 3���� �ڷ� ����
			else return 1; // ���Ű� �ƴ� ���, page 1�� �ڷ� ����
		}

		cout << endl;

		vector<Product>* sideList = data->getCategoryArray()[1];
		vector<Product>* drinkList = data->getCategoryArray()[3];

		if (side == true && input == 'a') { // ���̵� ����
			cout << "�� ���̵� ���� ��" << endl;

			for (unsigned int i = 0; i < sideList->size(); i++) // ���̵� ī�װ� ���
			{
				cout << "  " << (char)(i + 97) << ". " << sideList->at(i).getName() << endl;
			}

			while (1) {
				cout << "\n - �Է� : ";
				inputs();
				if (input >= 'a' && input <= 'a' + sideList->size() - 1) {
					Product newProduct(sideList->at(input - 'a'));
					vector<Product>& currentCart = data->getCurrentCart();
					cout << currentCart.size() << endl;
					switch (option) //burger, side, dessert, drink
					{
					case 'a': //burger
						if (comboAndSet == 'c')
							currentCart.at(1) = newProduct;
						break;
						/*case 'b': //side�� ���.
							currentCart.at(1) = newProduct;
							break;*/
					default:
						break;
					}
					break;
				} // ��Ͽ� ���� ������ �Է½� ���Է�
				gotoxy(0, 13 + sideList->size());
				line_clear(); // �� �� �����
			}
		}
		else if (input == 'b' || (side == false && input == 'a')) { // ���� ����
			cout << "�� ���� ���� ��" << endl;

			for (unsigned int i = 0; i < drinkList->size(); i++) // ���� ī�װ� ���
			{
				cout << "  " << (char)(i + 97) << ". " << drinkList->at(i).getName() << endl;
			}

			while (1) {
				cout << "\n - �Է� : ";
				inputs();
				if (input >= 'a' && input <= 'a' + drinkList->size() - 1) {
					Product newProduct(drinkList->at(input - 'a'));
					vector<Product>& currentCart = data->getCurrentCart();
					switch (option) //burger, side, dessert, drink
					{
					case 'a': //burger
						if (comboAndSet == 'b') //���� + ����
							currentCart.at(1) = newProduct;
						else if (comboAndSet == 'c') //���� + ���� + ���̵�
							currentCart.at(2) = newProduct;
						break;
						/*case 'd': //drink�� ���.
							currentCart.at(0) = newProduct;
							break;*/
					default:
						break;
					}
					break;
				}
				gotoxy(0, 13 + drinkList->size());
				line_clear();
			}
		}
		console_clear();
	}
}

int ConsoleView::userUI5() { // īƮ Ȯ��
	while (1) {
		cout << "* īƮ �� ���(0)\n" << endl;

		cout << "�� īƮ Ȯ�� ��" << endl;

		vector<vector<Product>> cartList = data->getCartList(); // ��� īƮ ��� 

		border_above();
		for (unsigned int i = 0; i < cartList.size(); i++) // ��� īƮ ��� ���
		{
			cout << "�� " << i + 1 << ". ";
			print_all_elements(cartList.at(i));
			gotoxy(101, 4 + i);
			cout << "�� " << endl;
		}
		border_bottom();

		cout << "# : Ȯ��" << endl;

		cout << "\n - �Է� : ";
		inputs();
		console_clear();
		if (input == '#') return 6; // ���� ������ page 6���� �̵�
		else if (input == '0') return 0; // �ڷ� ����
	}
}

int ConsoleView::userUI6() { // ����-���� & ���� ����
	while (1) {
		cout << "* �ڷΰ���(0)\n" << endl;
		cout << "�� ���� �Ļ� / ���� ��" << endl;

		cout << "  a. ���� �Ļ�" << endl;
		cout << "  b. ����" << endl;

		cout << "\n - �Է� : ";
		inputs();
		char temp = input; // ���� ���� ���

		if (input == '0') {
			console_clear();
			return 5; // �ڷ� ����
		}
		if (input != 'a' && input != 'b') { // ���� ������ �Է½� ���Է�
			console_clear();
			continue;
		}

		cout << "\n * �����Ͻðڽ��ϱ�? (y/n)" << endl;
		inputs();

		console_clear();
		if (input == 'y') { // �����ϱ�
			if (temp == 'a') *(data->get_for_here()) += 1; // ���� �ֹ� Ƚ�� ���� 
			else *(data->get_to_go()) += 1; // ���� �ֹ� Ƚ�� ���� 
			for (int i = 0; i < data->getCartList().size(); i++) { // �ȸ� īƮ����Ʈ ���� 
				for (int j = 0; j < data->getCartList().at(i).size(); j++) { // īƮ����Ʈ�� īƮ ����

					fm->setTime("sold_list.txt");   // @ 
					fm->write("sold_list.txt","/");
					fm->write("sold_list.txt",data->getCartList().at(i).at(j).getName());  //@
					fm->write("sold_list.txt", "/");
					fm->write("sold_list.txt",to_string(data->getCartList().at(i).at(j).getPrice()));  //@

					for (int k = 0; k < data->getCartList().at(i).at(j).getMaterialList().size(); k++) { //īƮ�� ��ǰ ���� 
						data->getCartList().at(i).at(j).getMaterialList().at(k).decreaseStock(1); // ������ ���� ���� 
						//cout << data->getCartList().at(i).at(j).getMaterialList().at(k).getName() << data->getCartList().at(i).at(j).getMaterialList().at(k).getStock() << endl;
							

					}
					fm->write("sold_list.txt", "\n");  //@
				}
				data->getSoldCartList().push_back(data->getCartList().at(i)); // �ȸ� īƮ ����Ʈ�� �ȸ� īƮ�� ���� 
			}
			data->getCartList().clear(); // ���� �� īƮ����Ʈ ���� 
			return 0; // page 0���� �̵�
		}
	}
}

int ConsoleView::helpUI() {
	while (1) {
		cout << "* �ڷΰ���(0)\n" << endl;
		cout << "< ���� >" << endl;

		cout << "\n - �Է� : ";
		inputs();

		if (input == '0') {
			console_clear();
			return 0;
		}
	}
}

int ConsoleView::adminUI0() {
	while (1) {
		cout << "�� ������ ��� ��" << endl;
		cout << " 1. ����ǰ �߰�" << endl;
		cout << " 2. ���� ��ǰ ����" << endl;
		cout << " 3. ��� ��� �߰�" << endl;
		cout << " 4. ��� �ڷ� ��ȸ" << endl;
		cout << " 5. ����� ���\n" << endl;

		cout << " * Ű����ũ ����(E)" << endl;

		cout << "\n - �Է� : ";
		inputs();

		console_clear();
		// page �̵�
		if (input == '1') return 8;
		if (input == '2') return 9;
		if (input == '3') return 10;
		if (input == '4') return 11;
		if (input == '5') return 0;
		if (input == 'E') return -1;
	}
	return 0;
}

int ConsoleView::adminUI1() { // ����ǰ �߰�
	while (1) {
		cout << "* �ڷΰ���(0)\n" << endl;
		cout << "�� ����ǰ �߰� ��" << endl;
		cout << " 1. ����" << endl;
		cout << " 2. ���̵�" << endl;
		cout << " 3. ����Ʈ" << endl;
		cout << " 4. ����\n" << endl;
		cout << "\n - �Է� : ";
		inputs();
		cout << endl;

		if (input == '0') { // �ڷ� ����
			console_clear();
			return 7;
		}

		vector<Product>* selected_category = data->getCategoryArray()[0]; // �ʱ�ȭ (�ƹ��ų���)
		if (input == '1') selected_category = data->getCategoryArray()[0]; // ���� 
		else if (input == '2') selected_category = data->getCategoryArray()[1]; // ���̵� 
		else if (input == '3') selected_category = data->getCategoryArray()[2]; // ����Ʈ 
		else if (input == '4') selected_category = data->getCategoryArray()[3]; // ���� 

		cout << " - ��ǰ ��� -" << endl;
		for (unsigned int i = 0; i < selected_category->size(); i++) // ������ ī�װ� ��� ���
		{
			cout << "  * " << selected_category->at(i).getName() << endl;
		}

		string product_name = ""; // ����ǰ��
		cout << "\n �� �߰��� ����ǰ��(���� 2��) : ";
		getline(cin, product_name);
		cin.clear();
		cin.ignore();

		cout << "\n - ��� ��� -" << endl;
		vector<Material>* materialList = data->getMaterialCategory(); // 
		for (unsigned int i = 0; i < materialList->size(); i++) // ��� ��� ���
		{
			cout << "  " << (i + 1) << ". " << materialList->at(i).getName() << endl;
		}

		cout << "\n �� ����ǰ�� ������ ��� (����� ���� ���� ����) : ";

		string materials_input;
		getline(cin, materials_input); // ���� ������ tokenizing �� ����
		stringstream ss(materials_input);

		vector<int> material_indices; // �Ľ̵� token���� ������ ����
		string token;
		while (getline(ss, token, ' ')) { // tokening
			material_indices.push_back(atoi(token.c_str()) - 1);
		}

		vector<Material> materials_for_new;
		for (unsigned int i = 0; i < material_indices.size(); i++) // ����ǰ�� ������ ��� ����� ���Ϳ� ä���
		{
			materials_for_new.push_back(data->getMaterialCategory()->at(material_indices.at(i)));
		}
		selected_category->push_back(Product(product_name, materials_for_new)); // ����ǰ�� �ش� ī�װ��� �߰� 

		console_clear();
	}
}

int ConsoleView::adminUI2() { // ���� ��ǰ ����
	while (1) {
		cout << "* �ڷΰ���(0)\n" << endl;
		cout << "�� ���� ��ǰ ���� ��" << endl;
		cout << " 1. ����" << endl;
		cout << " 2. ���̵�" << endl;
		cout << " 3. ����Ʈ" << endl;
		cout << " 4. ����\n" << endl;
		cout << "\n - �Է� : ";
		inputs();
		cout << endl;

		if (input == '0') { // �ڷ� ����
			console_clear();
			return 7;
		}
		if (input <= '0' || input >= '5') { // �������� ���� �Է�
			console_clear();
			continue;
		}

		vector<Product>* selected_category = data->getCategoryArray()[0]; // �ʱ�ȭ (�ƹ��ų�)
		if (input == '1') selected_category = data->getCategoryArray()[0]; // ���� 
		else if (input == '2') selected_category = data->getCategoryArray()[1]; // ���̵� 
		else if (input == '3') selected_category = data->getCategoryArray()[2]; // ����Ʈ 
		else if (input == '4') selected_category = data->getCategoryArray()[3]; // ���� 

		for (unsigned int i = 0; i < selected_category->size(); i++) // ��ǰ ��� ���
		{
			cout << "  " << (i + 1) << ". " << selected_category->at(i).getName() << endl;
		}

		cout << endl;

		while (1) {
			cout << " - �Է� : ";
			inputs();

			int set; char * toss;
			toss = &input;
			set = atoi(toss);

			if (input >= '1' && input <= '1' + selected_category->size() - 1) {
				selected_category->erase(selected_category->begin() + set - 1);

				break; } // �������� �ִ� ��ȣ�� �Է��ؾ�, break
			gotoxy(0, 12 + selected_category->size());
			line_clear();
		}

		console_clear();
	}
}

int ConsoleView::adminUI3() { // ��� ��� �Է�
	while (1) {
		cout << "* �ڷΰ���(0)\n" << endl;
		cout << "�� ��� ��� �߰� ��" << endl;
		vector<Material>* materialCategory = data->getMaterialCategory(); // ��� ī�װ� 
		for (unsigned int i = 0; i < materialCategory->size(); i++)
		{
			cout << "  " << (i + 1) << ". " << materialCategory->at(i).getName()
				<< " : " << materialCategory->at(i).getStock() << "��" << endl;
		}

		cout << "\n - �߰��� ��� : ";
		int num;
		cin >> num;

		if (num >= 1 && num <= 1 + materialCategory->size() - 1) { // ��� ����
			cout << " - �߰��� ���� : ";
			int stock;
			cin >> stock;
			cin.clear();
			cin.ignore(1, '\n');
			materialCategory->at(num - 1).increaseStock(stock); // ��� �߰�
			console_clear();
		}
		else if (num == 0) { // �ڷ� ����
			console_clear();
			return 7;
		}
		console_clear();
	}
	return 0;
}

int ConsoleView::adminUI4() { // ��� �ڷ� ��ȸ
	while (1) {
		cout << "�� ��� �ڷ� ��ȸ ��\n" << endl;
		cout << " 1. �� ����� : " << 0000 << "��" << endl;
		cout << "\n 2. ���� �Ǹ� : " << 00 << "ȸ" << endl;
		cout << "    ���� �Ǹ� : " << 00 << "ȸ" << endl;
		cout << "\n 3. ��ǰ�� �Ǹ� ��Ȳ" << endl; // ��ǰ ����, �Ǹ� ����, �Ǹ� ����

		cout << "\n  ����ǰ��"; gotoxy(22, 9); cout << "�װ���"; gotoxy(32, 9); cout << "���Ǹ� ����"; gotoxy(45, 9); cout << "����ǰ ����" << endl;

		// for���� ������ ����ϵ�, gotoxy�� y��ǥ�� 10���� 1�� ����
		cout << "    " << "����� �����"; gotoxy(24, 10); cout << "2000��"; gotoxy(34, 10); cout << "12��"; gotoxy(47, 10); cout << "24000��" << endl;
		cout << "    " << "ġ�� ����"; gotoxy(24, 11); cout << "3000��"; gotoxy(34, 11); cout << "50��"; gotoxy(47, 11); cout << "150000��" << endl;

		cout << "\n 4. �ð��뺰 �˻� : " << endl;
		cout << " * �ڷ� ������ 0, �˻��Ϸ��� 1 �Է� : ";
		inputs();

		if (input == '0') { // �ڷ� ����
			console_clear();
			return 7;
		}
		else if (input == '1') { // �˻�
			console_clear();
		}
		else { // �߸��� �Է�
			console_clear();
			continue;
		}

		while (1) {
			int month_from, day_from, hour_from, min_from;
			int month_to, day_to, hour_to, min_to;
			cout << "  < From >" << endl;
			cout << "   - �� : "; cin >> month_from;
			cout << "   - �� : "; cin >> day_from;
			cout << "   - �� : "; cin >> hour_from;
			cout << "   - �� : "; cin >> min_from;
			cout << "\n  < To >" << endl;
			cout << "   - �� : "; cin >> month_to;
			cout << "   - �� : "; cin >> day_to;
			cout << "   - �� : "; cin >> hour_to;
			cout << "   - �� : "; cin >> min_to;

			// ��� ���
			cout << "\n�� " << month_from << "�� " << day_from << "�� " << hour_from << "�� " << min_from << "�� ~ "
				<< month_to << "�� " << day_to << "�� " << hour_to << "�� " << min_to << "��" << endl;

			cout << "\n  ����ǰ��"; gotoxy(22, 14); cout << "�װ���"; gotoxy(32, 14); cout << "���Ǹ� ����";
			gotoxy(45, 14); cout << "����ǰ ����" << endl; gotoxy(58, 14); cout << "���Ǹ� �ð�" << endl;

			// for���� ������ ����ϵ�, gotoxy�� y��ǥ�� 15���� 1�� ����
			cout << "    " << "����� �����"; gotoxy(24, 15); cout << "2000��"; gotoxy(34, 15); cout << "12��";
			gotoxy(47, 15); cout << "24000��" << endl; gotoxy(60, 15); cout << "11/24 13:45" << endl;

			cout << "    " << "ġ�� ����"; gotoxy(24, 16); cout << "3000��"; gotoxy(34, 16); cout << "50��";
			gotoxy(47, 16); cout << "150000��" << endl; gotoxy(60, 16); cout << "11/25 09:03" << endl;

			cout << "\n * �ڷ� ������ 0, �ٽ� �˻��Ϸ��� 1 �Է� : ";
			inputs();
			if (input == '0') { // �ڷ� ����
				console_clear();
				break;
			}
			else { // �ٽ� �˻�
				console_clear();
			}
		}
	}
}

void ConsoleView::print_all_elements(vector<Product>& list) {
	if (list.size() == 0) return;
	for (unsigned int i = 0; i < list.size() - 1; i++)
	{
		cout << list.at(i).getName() << " + ";
	}
	cout << list.at(list.size() - 1).getName();
}

void ConsoleView::line_clear() {
	cout << "\33[2K";
}

void ConsoleView::console_clear() {
	system("cls");
}

void ConsoleView::border_above() {
	cout << "��";
	for (int i = 0; i < 100; i++)
	{
		cout << "��";
	}
	cout << "��" << endl;
}

void ConsoleView::border_bottom() {
	cout << "��";
	for (int i = 0; i < 100; i++)
	{
		cout << "��";
	}
	cout << "��" << endl;
}

void ConsoleView::pay() {
	cout << "��";
	for (int i = 0; i < 12; i++)
	{
		cout << "��";
	}
	cout << "��" << endl;

	cout << "�� $. �����ϱ⦢" << endl;

	cout << "��";
	for (int i = 0; i < 12; i++)
	{
		cout << "��";
	}
	cout << "��" << endl;
}

void ConsoleView::inputs() {
	cin >> input;
	cin.clear();
	cin.ignore(1, '\n');
}