# YAGE Package file format library

> Inspired by 
> - ``iff`` file format by EA 
> - ``esm``/``esp`` file formats from The Elder Scrolls series games

--------------

There are two major package types : 
- Master - used for game or major addon data. They are self contained and don't rely on other packages.
- Plugin - used for patches, smaller addons (like DLCs) and mods created by users. They are considerably smaller, because they only add or overwrite content from Master packages.
