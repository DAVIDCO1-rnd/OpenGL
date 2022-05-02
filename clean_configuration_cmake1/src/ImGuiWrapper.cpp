//#include "ImGuiWrapper.h"
//
//
//
//void ImGuiWrapper::displayImGui(size_t& modelIndex, std::vector<Model*> models, float* clearColor, bool& renderMesh) {
//	// Start the Dear ImGui frame
//	ImGui_ImplOpenGL3_NewFrame();
//	ImGui_ImplGlfw_NewFrame();
//
//	ImGui::NewFrame();
//
//	std::string modelName = models[modelIndex]->getModelName();
//	ImGui::Begin(modelName.c_str());
//	{
//		ImGui::ColorEdit3("clear color", clearColor); // Edit 3 floats representing a color
//
//		std::vector<std::string> items;
//		for (size_t i = 0; i < models.size(); i++)
//		{
//			items.push_back(models[i]->getModelName());
//		}
//		//string items[] = { meshes[0].getModelName(), meshes[1].getModelName()};
//
//		if (ImGui::BeginListBox("models"))
//		{
//			for (size_t n = 0; n < items.size(); n++)
//			{
//				const bool is_selected = (modelIndex == n);
//				if (ImGui::Selectable(items[n].c_str(), is_selected))
//					modelIndex = n;
//
//				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
//				// if (is_selected)
//				//     ImGui::SetItemDefaultFocus();
//			}
//			ImGui::EndListBox();
//		}
//
//		{
//			ModelParameters meshParams = models[modelIndex]->getParams();
//
//			meshParams.angleX = models[modelIndex]->getParams().angleX;
//			meshParams.angleY = models[modelIndex]->getParams().angleY;
//			meshParams.angleZ = models[modelIndex]->getParams().angleZ;
//
//			meshParams.translateX = models[modelIndex]->getParams().translateX;
//			meshParams.translateY = models[modelIndex]->getParams().translateY;
//			meshParams.translateZ = models[modelIndex]->getParams().translateZ;
//
//			meshParams.scaleX = models[modelIndex]->getParams().scaleX;
//			meshParams.scaleY = models[modelIndex]->getParams().scaleY;
//			meshParams.scaleZ = models[modelIndex]->getParams().scaleZ;
//
//			meshParams.scaleUniform = models[modelIndex]->getParams().scaleUniform;
//
//			ImGui::InputFloat("rotate X", &meshParams.angleX, 1.0f, 1.0f);
//			ImGui::InputFloat("rotate Y", &meshParams.angleY, 1.0f, 1.0f);
//			ImGui::InputFloat("rotate Z", &meshParams.angleZ, 1.0f, 1.0f);
//
//			ImGui::InputFloat("translate X", &meshParams.translateX, 0.1f, 1.0f);
//			ImGui::InputFloat("translate Y", &meshParams.translateY, 0.1f, 1.0f);
//			ImGui::InputFloat("translate Z", &meshParams.translateZ, 0.1f, 1.0f);
//
//			ImGui::InputFloat("scale X", &meshParams.scaleX, 0.01f, 1.0f);
//			ImGui::InputFloat("scale Y", &meshParams.scaleY, 0.01f, 1.0f);
//			ImGui::InputFloat("scale Z", &meshParams.scaleZ, 0.01f, 1.0f);
//			ImGui::InputFloat("uniform scale", &meshParams.scaleUniform, 0.01f, 1.0f);
//			ImGui::Checkbox("render mesh", &renderMesh);
//
//			models[modelIndex]->setParams(meshParams);
//		}
//	}
//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//	ImGui::End();
//}
//
//void ImGuiWrapper::initializeImGui(GLFWwindow* window, const char* glsl_version) {
//	// Setup Dear ImGui context
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//
//	// Setup Dear ImGui style
//	ImGui::StyleColorsDark();
//	//ImGui::StyleColorsClassic();
//
//	// Setup Platform/Renderer backends
//	ImGui_ImplGlfw_InitForOpenGL(window, true);
//	ImGui_ImplOpenGL3_Init(glsl_version);
//}
//
//void ImGuiWrapper::render() {
//	ImGui::Render();
//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//}
//
//void ImGuiWrapper::cleanUp() {
//	ImGui_ImplOpenGL3_Shutdown();
//	ImGui_ImplGlfw_Shutdown();
//	ImGui::DestroyContext();
//}