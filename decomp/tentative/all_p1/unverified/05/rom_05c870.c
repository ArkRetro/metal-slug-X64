/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05c870.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5c870(void)

{
  short sVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 in_CF;
  undefined1 uVar4;

  do {
    candidate_5e720();
    candidate_5e6ec();
    if (!(bool)in_CF) {
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0x10000;
      candidate_5e73a();
      candidate_4256();
      *unaff_A6 = 0x5c89e;
      uVar4 = 0;
      candidate_5e1e4();
      candidate_427a();
      candidate_5e26c();
      if ((bool)uVar4) {
        *unaff_A6 = 0x5c91c;
        uVar4 = false;
        candidate_5e60c();
        if ((bool)uVar4) {
          *unaff_A6 = candidate_5ca76;
          uVar4 = 0;
        }
      }
      candidate_5e454();
      if ((bool)uVar4) {
        *unaff_A6 = 0x5c694;
        uVar4 = 0;
      }
      candidate_5e5ba();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_5cc94;
        uVar4 = 0;
      }
      candidate_5e4f0();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_5c9c0;
        uVar4 = 0;
      }
      candidate_5e6ec();
      if ((bool)uVar4) {
        *unaff_A6 = 0x5c91c;
        uVar4 = 0;
      }
      candidate_5e536();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_5cb72;
        uVar4 = 0;
      }
      candidate_5e244();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_5cd08;
      }
      goto LAB_0005dab4;
    }
    candidate_5e720();
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) ^ 1;
    candidate_5e6ec();
  } while ((bool)in_CF);
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0x10000;
  candidate_5e73a();
  candidate_4256();
  *unaff_A6 = 0x5c950;
  uVar4 = 0;
  candidate_5e1e4();
  candidate_427a();
  candidate_5e26c();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5c870;
    uVar4 = 0;
  }
  candidate_5e454();
  if ((bool)uVar4) {
    *unaff_A6 = 0x5c694;
    uVar4 = 0;
  }
  candidate_5e5ba();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5cc94;
    uVar4 = 0;
  }
  candidate_5e4f0();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5c9c0;
    uVar4 = 0;
  }
  candidate_5e6ec();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5c870;
    uVar4 = 0;
  }
  candidate_5e536();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5cb72;
    uVar4 = 0;
  }
  candidate_5e244();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_5cd08;
  }
LAB_0005dab4:
  candidate_5e6d2();
  sVar1 = *(short *)(unaff_A6 + 0x1f);
  sVar2 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar2;
  if (-1 >= sVar2) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar3 = (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0;
  if (!bVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x1e;
  }
  bVar3 = bVar3 && (-1 < sVar2 && sVar1 == 0);
  candidate_5e7a8();
  if (bVar3) {
    *unaff_A6 = candidate_5d732;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db18;
  }
  bVar3 = false;
  candidate_1945c();
  if (bVar3) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
