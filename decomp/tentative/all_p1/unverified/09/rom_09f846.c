/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f846.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f846(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  if ((*(byte *)(unaff_A6 + 0x24) & 2) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x7a) = 0xa039e;
    *unaff_A6 = 0x9f878;
    uVar1 = 0;
    candidate_a0576();
    candidate_a0356();
    candidate_a04a8();
    if (((bool)uVar1) && (*(int *)((int)unaff_A6 + 0x7a) != 0xa043a)) {
      *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
      *(undefined4 *)((int)unaff_A6 + 0x7a) = 0xa043a;
    }
    uVar1 = *(uint *)((int)unaff_A6 + 0x7a) < 0xa043a;
    if (*(uint *)((int)unaff_A6 + 0x7a) != 0xa043a) {
      candidate_a04e2();
      if ((bool)uVar1) {
        *unaff_A6 = candidate_9f90a;
        uVar1 = 0;
      }
      candidate_1af9a();
      if ((bool)uVar1) {
        *unaff_A6 = 0x9f968;
        uVar1 = 0;
      }
    }
    candidate_427a();
    candidate_1af9a();
    if (!(bool)uVar1) {
      candidate_a059a();
      if ((bool)uVar1) {
        *unaff_A6 = candidate_9fb48;
        uVar1 = 0;
      }
      candidate_a0664();
      if ((bool)uVar1) {
        *unaff_A6 = candidate_9fa48;
        uVar1 = 0;
      }
      candidate_a06e4();
      if ((bool)uVar1) {
        *unaff_A6 = candidate_9f9fa;
        uVar1 = 0;
      }
    }
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    if ((*(byte *)(unaff_A6 + 0x24) & 2) == 0) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *unaff_A6 = 0x9f9a6;
    uVar1 = 0;
    candidate_a0576();
    candidate_a0334();
    candidate_427a();
    candidate_1af9a();
    if (!(bool)uVar1) {
      candidate_a059a();
      if ((bool)uVar1) {
        *unaff_A6 = candidate_9fb48;
        uVar1 = 0;
      }
      candidate_a0664();
      if ((bool)uVar1) {
        *unaff_A6 = candidate_9fa48;
        uVar1 = 0;
      }
      candidate_a06e4();
      if ((bool)uVar1) {
        *unaff_A6 = candidate_9f9fa;
        uVar1 = 0;
      }
      candidate_a074a();
      if ((bool)uVar1) {
        *unaff_A6 = candidate_9fa48;
        uVar1 = 0;
      }
    }
  }
  if ((*(byte *)(unaff_A6 + 0x24) & 0x80) == 0) {
    *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
    uVar1 = 0;
    if (*(short *)(unaff_A6 + 0x20) < 1) {
      *unaff_A6 = candidate_9fc9c;
      uVar1 = 0;
    }
  }
  candidate_a072a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_9fade;
    uVar1 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = 0x9fca6;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_9fd60;
  }
  return;
}
