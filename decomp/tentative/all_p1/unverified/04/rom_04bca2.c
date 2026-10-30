/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04bca2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4bca2(void)

{
  short sVar1;
  int iVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(undefined2 *)(unaff_A6 + 0x21) = 0;
  sVar1 = candidate_29f2();
  if (*(char *)(unaff_A6 + 0x29) == '\0') {
    iVar2 = 0x2c25e2;
  }
  else {
    iVar2 = 0x2c2602;
  }
  *(undefined4 *)((int)unaff_A6 + 0x92) = *(undefined4 *)(iVar2 + (short)(sVar1 << 2));
  *(undefined1 *)((int)unaff_A6 + 0x9b) =
       *(undefined1 *)
        (*(int *)((int)unaff_A6 + 0x92) + (int)(short)(*(short *)(unaff_A6 + 0x21) * 2));
  if (*(char *)((int)unaff_A6 + 0x9b) == *(char *)((int)unaff_A6 + 0x9a)) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  if (*(char *)(unaff_A6 + 0x27) == '\0') {
    *(undefined1 *)(unaff_A6 + 0x27) = 8;
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x27) = 0;
  }
  *(undefined1 *)((int)unaff_A6 + 0x9f) = 0;
  *unaff_A6 = 0x4bd32;
  uVar3 = 0;
  candidate_1abc8();
  candidate_4d9de();
  if ((bool)uVar3) {
    if ((*(byte *)((int)unaff_A6 + 0x9f) & 1) == 0) {
      *(byte *)((int)unaff_A6 + 0x9f) = *(byte *)((int)unaff_A6 + 0x9f) | 1;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x9f) = *(byte *)((int)unaff_A6 + 0x9f) & 0xfe;
    }
    candidate_4d768();
    if ((bool)uVar3) {
      if ((*(byte *)((int)unaff_A6 + 0x9f) & 2) == 0) {
        *(byte *)((int)unaff_A6 + 0x9f) = *(byte *)((int)unaff_A6 + 0x9f) | 2;
        if (*(char *)(unaff_A6 + 0x27) == '\0') {
          *(undefined1 *)(unaff_A6 + 0x27) = 8;
        }
        else {
          *(undefined1 *)(unaff_A6 + 0x27) = 0;
        }
        *(undefined1 *)((int)unaff_A6 + 0x9b) =
             *(undefined1 *)
              (*(int *)((int)unaff_A6 + 0x92) + 1 + (int)(short)(*(short *)(unaff_A6 + 0x21) * 2));
      }
      else {
        *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + 1;
        if (*(short *)(unaff_A6 + 0x21) < *(short *)((int)unaff_A6 + 0x82)) {
          *unaff_A6 = 0x4bcd2;
        }
        else {
          *unaff_A6 = candidate_4bf66;
        }
      }
    }
    if (*(char *)((int)unaff_A6 + 0x9b) == *(char *)((int)unaff_A6 + 0x9a)) {
      uVar3 = 0;
      candidate_4256();
    }
    else {
      uVar3 = 0;
      candidate_4256();
    }
  }
  candidate_4d74a();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    *unaff_A6 = candidate_4be04;
  }
  return;
}
