using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Inventory : MonoBehaviour
{
    public List<Item> items = new List<Item>();
    public static Inventory instance;

    public int space = 20;

    public delegate void OnItemChanged();
    public OnItemChanged OnItemChangedCallback;

    void Awake()//define the inventory instance
    {
        instance = this;
    }

    public bool Add(Item item)
    {
        if (!item.isDefaultItem)//if not default item then add
        {
            if (items.Count >= space)
            {
                return false;
            }

            items.Add(item);
            if (OnItemChangedCallback != null)
                OnItemChangedCallback.Invoke();


        }
        return true;
    }

    public void Remove(Item item)
    {
        items.Remove(item);
        if (OnItemChangedCallback != null)
            OnItemChangedCallback.Invoke();
    }
}
