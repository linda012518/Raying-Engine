#include "hzpch.h"
#include "LayerStack.h"

namespace Raying
{
	LayerStack::LayerStack()
	{

	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : _layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		_layers.emplace(_layers.begin() + _layerInsert, layer);
		_layerInsert++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		_layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto itr = std::find(_layers.begin(), _layers.begin() + _layerInsert, layer);
		if (itr != _layers.end())
		{
			layer->OnDetach();
			_layers.erase(itr);
			_layerInsert--;
		}
	}

	void LayerStack::PopOverLay(Layer* overlay)
	{
		auto itr = std::find(_layers.begin(), _layers.begin() + _layerInsert, overlay);
		if (itr != _layers.end())
		{
			overlay->OnDetach();
			_layers.erase(itr);
		}

	}

}