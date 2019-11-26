#include "ConsoleView.h"
#include "Product.h"
#include "Data.h"
#include "FileManager.h"

int main() {
	FileManager* fm = new FileManager();
	Data* data = new Data(fm);
	ConsoleView view(data, fm);
	view.showUI();
	return 0;
}
