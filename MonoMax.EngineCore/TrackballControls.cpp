#include "TrackballControls.h"
#include "..//packages/glm.0.9.9.500/build/native/include/glm/gtc/quaternion.hpp"
#include "..//packages/glm.0.9.9.500/build/native/include/glm/gtx/norm.hpp"
#include "..//packages/glm.0.9.9.500/build/native/include/glm/gtc/matrix_transform.hpp"

#define   FloatInfinity std::numeric_limits<float>::infinity()
#define   SQRT1_2  0.7071067811865476

#ifdef _WIN32 // GNU GCC compiler has 'isnan' built in */
inline bool isnan(double num) { return num != num; /* (bool)_isnan(num);*/ }
#endif

namespace MonoMaxGraphics
{

	uint32_t TrackballControls::m_keys[3] = { 65 /*A*/, 83 /*S*/, 68 /*D*/ };

	static TrackballControls* s_instance = nullptr;


	Camera3D::Camera3D(const glm::vec3& pos)
		:m_eye(pos), m_up(0.0f, 1.0f, 0.0f), m_viewMatr(1.0f)
	{


	}
	void Camera3D::LookAt(const glm::vec3& target) {

		m_viewMatr = glm::lookAt(m_eye, target, m_up);
	}


	////------------------------   Static callbacks -------------------------------------------------
	//static void MouseButtonCallBack(GLFWwindow* win, int button, int action, int mods) {
	//	if (action == GLFW_PRESS) {
	//		double xpos, ypos;
	//		glfwGetCursorPos(win, &xpos, &ypos);
	//		s_instance->MouseDown(button, action, mods, (int)xpos, (int)ypos);
	//	}
	//	if (action == GLFW_RELEASE) {
	//		s_instance->MouseUp();
	//	}

	//}
	//static void MouseMoveCallBack(GLFWwindow* win, double xpos, double ypos) {
	//	s_instance->MouseMove((int)xpos, (int)ypos);
	//}

	//static void MouseScrollCallBack(GLFWwindow* win, double xpos, double ypos) {

	//	s_instance->MouseWheel(xpos, ypos);

	//}
	//static void KeyboardCallBack(GLFWwindow* win, int key, int scancode, int action, int mods) {
	//	if (action == GLFW_PRESS) {
	//		s_instance->KeyDown(key);
	//	}

	//	if (action == GLFW_RELEASE) {
	//		s_instance->KeyUp();
	//	}

	//}
	//-----------------------------------------------------------------------------------------------


	TrackballControls::TrackballControls(Camera3D* cam, const int width, const int height)
		:m_pCam(cam),
		mWidth(width),
		mHeight(height),
		m_enabled(true),
		m_rotateSpeed(2.2f),
		m_zoomSpeed(1.2f),
		m_panSpeed(0.3f),
		m_noRotate(false),
		m_noPan(false),
		m_noZoom(false),
		m_noRoll(true),
		m_staticMoving(true),
		m_dynamicDampingFactor(0.2f),
		m_minDistance(0.0f),
		m_maxDistance(FloatInfinity),
		m_target(0.0f),
		m_lastPos(0.0f),
		m_state(TCB_STATE::NONE),
		m_prevState(TCB_STATE::NONE),
		m_eye(0.0f),
		m_rotStart(0.0f),
		m_rotEnd(0.0f),
		m_zoomStart(0.0f),
		m_zoomEnd(0.0f),
		m_panStart(0.0f),
		m_panEnd(0.0f)
	{


	}

	void TrackballControls::Init(Camera3D* camera, const int width, const int height) 
	{
		SetSize(width, height);
		//glfwSetCursorPosCallback(win, &MouseMoveCallBack);
		//glfwSetMouseButtonCallback(win, &MouseButtonCallBack);
		//glfwSetScrollCallback(win, &MouseScrollCallBack);
		//glfwSetKeyCallback(win, &KeyboardCallBack);
	}



	void TrackballControls::Update() {

		//   _eye.setFrom( object.position ).sub( target );
		m_eye = m_pCam->m_eye - m_target;


		//	m_eye -= m_eye  - m_target  ; // Vector3.Subtract(_eye, _target);
		if (!m_noRotate)
		{
			RotateCamera();
		}

		if (!m_noZoom)
		{
			ZoomCamera();
		}

		if (!m_noPan)
		{
			PanCamera();
		}

		// object.position =  target + _eye;
		m_pCam->m_eye = (m_target + m_eye);

		CheckDistances();

		m_pCam->LookAt(m_target);


		// distanceToSquared

		if (glm::length2(m_lastPos - m_pCam->m_eye) > 0.0f)
		{
			m_lastPos = m_pCam->m_eye;
		}
	}

	void TrackballControls::SetSize(const int width, const int height)
	{
		mWidth = width;
		mHeight = height;
	}

	void TrackballControls::RotateCamera()
	{


		float angle = (float)acos(glm::dot(m_rotStart, m_rotEnd) / glm::length(m_rotStart) / glm::length(m_rotEnd));

		if (!isnan(angle) && angle != 0.0f)
		{


			glm::vec3 axis = glm::normalize(glm::cross(m_rotStart, m_rotEnd)); //_rotateStart.cross(_rotateEnd).normalize();
			//   axis.Normalize();
			//axis.y*=-1.0f;///need to invert y as rotate seems to be in opposite to mouse move direction.
			if (glm::isnan(axis.x) || glm::isnan(axis.y) || glm::isnan(axis.z))
				return;

			glm::quat quaternion;// Quaternion quaternion = Quaternion.Identity;

			angle *= m_rotateSpeed;

			quaternion = glm::angleAxis(-angle, axis);
			//  quaternion.setAxisAngle(axis, angle);
			// quaternion = Quaternion.FromAxisAngle(axis, -angle);

			//quaternion.rotate(_eye);
			//   _eye = Vector3.Transform(_eye, quaternion);

			m_eye = glm::rotate(quaternion, m_eye);



			//  quaternion.rotate(object.up);
			m_pCam->m_up = glm::rotate(quaternion, m_pCam->m_up);
			//  quaternion.rotate(_rotateEnd);
			m_rotEnd = glm::rotate(quaternion, m_rotEnd);


			if (m_staticMoving)
			{

				m_rotStart = m_rotEnd;

			}
			else
			{

				quaternion = glm::angleAxis(angle * (m_dynamicDampingFactor - 1.0f), axis);


				m_rotStart = glm::rotate(quaternion, m_rotStart);

			}



		}

	}


	void TrackballControls::ZoomCamera()
	{

		float factor = 1.0f + (float)(m_zoomEnd.y - m_zoomStart.y) * m_zoomSpeed;

		if (factor != 1.0f && factor > 0.0f)
		{



			m_eye = m_eye * (float)factor;

			if (m_staticMoving)
			{

				m_zoomStart = m_zoomEnd;

			}
			else
			{

				m_zoomStart.y += (float)(m_zoomEnd.y - m_zoomStart.y) * m_dynamicDampingFactor;

			}

		}



	}

	void TrackballControls::PanCamera()
	{

		glm::vec2 mouseChange = m_panEnd - m_panStart;

		if (glm::length(mouseChange) != 0.0f)
		{

			// mouseChange.scale( _eye.Length * _panSpeed );
			mouseChange *= glm::length(m_eye) * m_panSpeed;

			//   Vector3 pan = _eye.cross( object.up ).normalize().scale( mouseChange.x );
			glm::vec3 pan = glm::normalize(glm::cross(m_eye, m_pCam->m_up));

			pan *= mouseChange.x;

			// pan += object.up.clone().normalize().scale( mouseChange.Y );
			glm::vec3 camUpClone = glm::normalize(m_pCam->m_up);

			camUpClone *= mouseChange.y;
			pan += camUpClone;

			//object.position.add( pan );


			m_pCam->m_eye += pan;



			m_target += pan;
			if (m_staticMoving)
			{

				m_panStart = m_panEnd;

			}
			else
			{

				m_panStart += (m_panEnd - m_panStart) * m_dynamicDampingFactor; //  (_panEnd - _panStart).scale(_dynamicDampingFactor);

			}

		}



	}

	void TrackballControls::CheckDistances()
	{

		if (!m_noZoom || !m_noPan)
		{



			if (glm::length2(m_pCam->m_eye) > m_maxDistance * m_maxDistance)
			{


				//  _camObject._pos.Normalize();

				//  _camObject._pos = Vector3.Multiply(_camObject._pos, _maxDistance);

				m_pCam->m_eye = glm::normalize(m_pCam->m_eye) * m_maxDistance;
			}

			if (glm::length2(m_eye) < m_minDistance * m_minDistance)
			{

				// object.position = target + _eye.normalize().scale(minDistance);

				m_eye = glm::normalize(m_eye) * m_minDistance;

				// _camObject._pos = _target + _eye;
				m_pCam->m_eye = m_target + m_eye;

			}

		}

	}

	glm::vec3 TrackballControls::GetMouseProjectionOnBall(int clientX, int clientY)
	{
		glm::vec3 mouseOnBall = glm::vec3(
			((float)clientX - (float)mWidth * 0.5f) / (float)(mWidth * 0.5f),
			((float)mHeight * 0.5f - (float)clientY) / (float)(mHeight * 0.5f),
			0.0f
		);

		double length = (double)glm::length(mouseOnBall);

		if (m_noRoll)
		{

			if (length < SQRT1_2)
			{

				mouseOnBall.z = (float)sqrt(1.0 - length * length);

			}
			else
			{

				mouseOnBall.z = (float)(0.5 / length);

			}

		}
		else if (length > 1.0)
		{

			mouseOnBall = glm::normalize(mouseOnBall);

		}
		else
		{

			mouseOnBall.z = (float)sqrt(1.0 - length * length);

		}



		m_eye = m_target - m_pCam->m_eye;


		glm::vec3 upClone = m_pCam->m_up;
		upClone = glm::normalize(upClone);
		//object.up.clone().normalize().scale(mouseOnBall.y);
		glm::vec3 projection = upClone * mouseOnBall.y;






		//  projection.add(object.up.cross(_eye).normalize().scale(mouseOnBall.x));

		glm::vec3 cross = glm::normalize(glm::cross(m_pCam->m_up, m_eye));


		cross *= mouseOnBall.x;
		projection += cross;

		//  projection.add(_eye.normalize().scale(mouseOnBall.z));
		glm::vec3 eyeClone = glm::normalize(m_eye);

		projection += eyeClone * mouseOnBall.z;

		return projection;

	}

	void TrackballControls::MouseDown(int button, int xpos, int ypos) {
		if (!m_enabled) { return; }



		if (m_state == TCB_STATE::NONE)
		{

			if (button == 1)
			{
				m_state = TCB_STATE::ROTATE;
			}
			else if(button == 2)
			{
				m_state = TCB_STATE::PAN;
			}
			//   _state = e.Button;//  event.button;
		}

		if (m_state == TCB_STATE::ROTATE && !m_noRotate)
		{


			m_rotStart = GetMouseProjectionOnBall(xpos, ypos);
			m_rotEnd = m_rotStart;

		}
		else if (m_state == TCB_STATE::ZOOM && !m_noZoom)
		{

			m_zoomStart = GetMouseOnScreen(xpos, ypos);
			m_zoomEnd = m_zoomStart;


		}
		else if (m_state == TCB_STATE::PAN && !m_noPan)
		{

			m_panStart = GetMouseOnScreen(xpos, ypos);
			m_panEnd = m_panStart;



		}

	}

	void TrackballControls::KeyDown(int key) {

		if (!m_enabled) return;

		m_prevState = m_state;

		//   var state = OpenTK.Input.Keyboard.GetState();

		if (m_state != TCB_STATE::NONE)
		{

			return;

		}
		//else if (key == GLFW_KEY_A /* event.keyCode == keys[ STATE.ROTATE ]*/ && !m_noRotate)
		//{

		//	m_state = TCB_STATE::ROTATE;

		//}
		//else if (key == GLFW_KEY_S /* event.keyCode == keys[ STATE.ZOOM ]*/ && !m_noZoom)
		//{

		//	m_state = TCB_STATE::ZOOM;

		//}
		//else if (key == GLFW_KEY_D /* event.keyCode == keys[ STATE.PAN ]*/ && !m_noPan)
		//{

		//	m_state = TCB_STATE::PAN;

		//}
	}

	void TrackballControls::MouseWheel(double xoffset, double yoffset) {

		if (!m_enabled) { return; }


		float delta = 0.0f;



		if (yoffset != 0.0)
		{ // Firefox

			delta = (float)yoffset / 3.0f;

		}

		m_zoomStart.y += delta * 0.05f;
	}

	void TrackballControls::MouseMove(int xpos, int ypos) {
		if (!m_enabled) { return; }

		if (m_state == TCB_STATE::ROTATE && !m_noRotate)
		{

			m_rotEnd = GetMouseProjectionOnBall(xpos, ypos);

		}
		else if (m_state == TCB_STATE::ZOOM && !m_noZoom)
		{

			m_zoomEnd = GetMouseOnScreen(xpos, ypos);

		}
		else if (m_state == TCB_STATE::PAN && !m_noPan)
		{

			m_panEnd = GetMouseOnScreen(xpos, ypos);


		}

	}

}