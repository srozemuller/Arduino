#### Arduino Water Softener Sensor with MQTT and HomeAssistant

For controlling my irrigation system i've started building a sensor which is capturing the consumed liters. 
##### Components i've used:

- [Wemos D1 Mini   Pro](https://nl.aliexpress.com/item/32845084675.html?spm=a2g0o.productlist.0.0.447c2a6c9p3L8q&algo_pvid=0a8d7fc9-790a-437a-96c6-850c4fab697b&algo_expid=0a8d7fc9-790a-437a-96c6-850c4fab697b-1&btsid=0b0a187b15889643446401239e643b&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
- [PCB Solder board](https://nl.aliexpress.com/item/32902801591.html?spm=a2g0o.productlist.0.0.77673622KI40vP&algo_pvid=fdf9053c-e135-4342-87d9-c516c5c7fbc1&algo_expid=fdf9053c-e135-4342-87d9-c516c5c7fbc1-11&btsid=0b0a187b15889646114502037e643b&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
- [HC-SR04 Ultrasonic sensor](https://nl.aliexpress.com/item/32713522570.html?spm=a2g0o.productlist.0.0.13cd577bxbOgqO&algo_pvid=4449a9b4-07e2-4349-a83b-a1ae61a22760&algo_expid=4449a9b4-07e2-4349-a83b-a1ae61a22760-0&btsid=0b0a0ae216090553886214993e2ea7&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)

The Arduino [main.ino](https://github.com/srozemuller/Arduino/blob/main/water-softener/main.ino) file is in this repo folder
See the [watersoftener.yaml](https://github.com/srozemuller/hassio-config/blob/master/sensors/watersoftener.yaml) file for the [HomeAssistant](https://www.home-assistant.io/) code i've used.

#### Home-Assistant card example
![home-assistant cards](https://user-images.githubusercontent.com/43162899/103166897-eaca0f80-4826-11eb-9ef1-97596b54bf61.png)

#### Hardware
![HC-SR04 Front](https://user-images.githubusercontent.com/43162899/103166914-08977480-4827-11eb-87f4-6b2af8eca695.jpeg)

![Wemos](https://user-images.githubusercontent.com/43162899/103166925-21078f00-4827-11eb-9c00-81d8d9e8d61e.jpeg)


![Sensor places on reservoir](https://user-images.githubusercontent.com/43162899/103166941-41cfe480-4827-11eb-99ed-6fc3b368702a.jpeg)


#### Home-Assistant card setup
```yaml
type: vertical-stack
cards:
  - cards:
      - type: entity
        entity: sensor.salt_level_last_update
        name: Last seen
        icon: 'mdi:eye-outline'
    type: horizontal-stack
  - cards:
      - cards:
          - type: sensor
            entity: sensor.salt_level
            icon: 'mdi:car-coolant-level'
            graph: line
          - type: sensor
            entity: sensor.salt_level_percent
            graph: line
            icon: 'mdi:percent-outline'
        type: horizontal-stack
    type: vertical-stack    
