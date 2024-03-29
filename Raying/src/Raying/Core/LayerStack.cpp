#include "hzpch.h"
#include "LayerStack.h"

namespace Raying
{
	LayerStack::~LayerStack()
	{
		for (Layer* layer : _layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		_layers.emplace(_layers.begin() + _layerInsert, layer);
		_layerInsert++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		_layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto itr = std::find(_layers.begin(), _layers.begin() + _layerInsert, layer);
		if (itr != _layers.begin() + _layerInsert)
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