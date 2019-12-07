--31y65x
--41y64x

select

	--(om_x * 100 + om_y) as "om_id",

	--om_y,
	--om_x,

	(omt_y * 1000 + omt_x) as "omt_id",

	omt_y,
	omt_x,

	((omt_y - 1) * 1000 + omt_x - 0) as "omt_id_N",
	((omt_y + 0) * 1000 + omt_x + 1) as "omt_id_E",
	((omt_y + 1) * 1000 + omt_x + 0) as "omt_id_S",
	((omt_y - 0) * 1000 + omt_x - 1) as "omt_id_W",

	((omt_y - 1) * 1000 + omt_x - 1) as "omt_id_NW",
	((omt_y + 1) * 1000 + omt_x + 1) as "omt_id_SE",
	((omt_y + 1) * 1000 + omt_x - 1) as "omt_id_SW",
	((omt_y - 1) * 1000 + omt_x + 1) as "omt_id_NE",

	--land_use_code,
	--primary_road_type,

	(case when land_use_code is null then 0 else land_use_code end) as "lu_code",
	(case when primary_road_type is null then 0 else 1 end) as "is_road",
	(case when land_use_code = 20 then 1 else 0 end) as "is_water"

from
	omt
where
	om_y = 41 and
	om_x = 64
order by
	om_y,
	om_x,
	omt_y,
	omt_x
;