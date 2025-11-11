#include "ImGuiManager.h"
#ifdef USE_IMGUI
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#endif // USE_IMGUI
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SrvUavManager.h"

void ImGuiManager::Initialize(WinApp* winApp, DirectXCommon* dxCommon, SrvUavManager* srvUavManager){
	winApp_ = winApp;
	dxCommon_ = dxCommon;
	srvUavManager_ = srvUavManager;
#ifdef USE_IMGUI
	//ImGuiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	//Win32用初期化
	ImGui_ImplWin32_Init(winApp_->GetHwnd());

	ImGui_ImplDX12_Init(
		dxCommon_->GetDevice(),
		static_cast<int>(dxCommon_->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvUavManager_->GetDescriptorHeapForImGui(),
		srvUavManager_->GetDescriptorHeapForImGui()->GetCPUDescriptorHandleForHeapStart(),
		srvUavManager_->GetDescriptorHeapForImGui()->GetGPUDescriptorHandleForHeapStart()
	);
#endif // USE_IMGUI
}

void ImGuiManager::Finalize(){
#ifdef USE_IMGUI
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif // USE_IMGUI
}

void ImGuiManager::Begin(){
#ifdef USE_IMGUI
	//ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif // USE_IMGUI
}

void ImGuiManager::End(){
#ifdef USE_IMGUI
	//描画前準備
	ImGui::Render();
#endif // USE_IMGUI
}

void ImGuiManager::Draw(){
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvUavManager_->GetDescriptorHeapForImGui() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
#ifdef USE_IMGUI
	//描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
#endif // USE_IMGUI
}
