#include "main.h"
#include "Phong.cpp"
#include "Object3D.cpp"

int main(int argc, char* argv[]) {
  	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(50, 50);
  	MainWindow = glutCreateWindow("A Simple Ray Tracer");
	glutKeyboardFunc(keyBoard);
  	glutDisplayFunc(display);
	modifyValue();
	glutReshapeFunc(reshape);
 	glutMainLoop();
}
void modifyValue(){
	cout << "Properties of Reflective Sphere" << endl;
	cout << " \nDefault Value ==> Ir = 1, Kr = 2.5, Trasperant = 0.5 <== " << "\n" << endl;
	cout << "Do you want to modify Ir, Kr, Transperant. Enter (y/n): ";
	cin >> c;
	while (c != 'y' && c != 'Y' && c != 'n' && c != 'N'){
		cout << "Error! you can only enter y or n" << endl;
		cout << "Please re-enter (y or n): ";
		cin >> c;
	}
	if(c == 'n' || c == 'N'){
		displayObject();
	}
	else if(c == 'y' || c == 'Y'){
		cout << "Enter Ir: "; cin >> ir;
		cout << "Enter Kr: "; cin >> kr;
		cout << "Enter transparent: "; cin >> transparent;
		displayObject();
	}
}
void reDisplay(){
	glutPostWindowRedisplay(MainWindow);
}
Viewing vObj; scene objectScene;
RayTracer rt(objNum); 
void colorFunc(float x, float y, RGB& shade) {
	Ray ray = vObj.GetRay(x, y);
	shade = rt.RayShadowRefl(objectScene, ray, 0).gc(1); 
}
RGB* pixels = new RGB[sizes]; //
void display(){	
	auto t1 = std::chrono::high_resolution_clock::now(); int i = 0;
	while(i < sizes) {
		colorFunc(i % screen_width, i / screen_width, pixels[i]);
		i = i + 1;
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration_cast<std::chrono
			::milliseconds>(t2 - t1).count();
	float num = time / 1000;
	if(k == 0){
		cout << endl;
		string str = to_string(num);
		cout << "Program took ";
		printf("%.2f", num);
		cout << " seconds " << "to diplay" << endl;
		cout<< "\nPress q or Q on window screen to exit the program"<< endl;
	}
	k -=- 1;
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  	glDrawPixels(screen_width, screen_height, GL_RGB, GL_FLOAT, pixels);
	glFlush();
}
void reshape(int w, int h) {
	screen_width = w,
	screen_height = h; 
	sizes = w * h;
	pixels = new RGB[w * h];
	vObj.PixelSize(screen_width, screen_height);
	reDisplay();
}
void displayObject() {
	if(incIr == false){
		vObj.position = vecRay(-0.7, 0, 0.4);
		vObj.PixelSize(screen_width, screen_height);
		vObj.setDirection(vecRay(1.05, 0.0, -0.2));
		DrawObject(objectScene, ir, kr, transparent);
	}
}
void keyBoard(unsigned char key, int x, int y) {
	switch (key){ 
		case 'q':case 'Q': cout << "Bye Bye!!" << endl; exit(0); break;
		default:break;
	}
}


