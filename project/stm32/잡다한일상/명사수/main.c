void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    static uint32_t block_time = 0;  // 인터럽트를 블록할 기준 시간 저장
    static uint8_t is_blocking = 0; // 블록 상태를 나타내는 플래그

    uint32_t current_time = HAL_GetTick();

    // 블록 상태 확인
    if (is_blocking && (current_time - block_time < 500)) {
        // 현재 블록 상태이며 500ms 이내라면 인터럽트를 무시
        return;
    }

    // 블록 상태를 해제 (500ms 경과 시)
    if (current_time - block_time >= 500) {
        is_blocking = 0; // 블록 해제
    }

    // 인터럽트 핀에 따른 처리
    if (GPIO_Pin == han_Pin) {
        current_tick_1 = HAL_GetTick();
        int32_t diff_1 = (int32_t)(current_tick_1 - old_tick_1);
        if (diff_1 < 0) {
            diff_1 += UINT32_MAX + 1;
        }
        if (diff_1 > 200) {
            state++;
            old_tick_1 = current_tick_1;
        }
    } else if (GPIO_Pin == han2_Pin) {
        current_tick_2 = HAL_GetTick();
        int32_t diff_2 = (int32_t)(current_tick_2 - old_tick_2);
        if (diff_2 < 0) {
            diff_2 += UINT32_MAX + 1;
        }
        if (diff_2 > 200) {
            state_1++;
            old_tick_2 = current_tick_2;
        }
    } else if (GPIO_Pin == han3_Pin) {
        current_tick_3 = HAL_GetTick();
        int32_t diff_3 = (int32_t)(current_tick_3 - old_tick_3);
        if (diff_3 < 0) {
            diff_3 += UINT32_MAX + 1;
        }
        if (diff_3 > 200) {
            state_2++;
            old_tick_3 = current_tick_3;
        }
    }

    // 인터럽트 처리 완료 후 다른 인터럽트를 블록 상태로 설정
    is_blocking = 1;
    block_time = HAL_GetTick();  // 블록 시작 시간 설정
}
