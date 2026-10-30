/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03965c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3965c(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  unaff_A6[0x14] = 0x39fe0;
  uVar2 = candidate_39eda();
  *(undefined2 *)((int)unaff_A6 + 0x82) = uVar2;
  *(undefined2 *)(unaff_A6 + 0x21) = uVar2;
  *(undefined1 *)(unaff_A6 + 0x24) = 0;
  if (*(short *)(unaff_A6 + 0x1c) == *(short *)(unaff_A6 + 0x1f)) {
    candidate_39f06();
    candidate_39eec();
    candidate_3971c();
    *unaff_A6 = 0x39694;
    uVar3 = 0;
    candidate_39d74();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
        *unaff_A6 = candidate_39796;
        uVar3 = 0;
      }
      candidate_39e9a();
      if ((bool)uVar3) {
        uVar3 = 0;
        if (*(short *)((int)unaff_A6 + 0x72) == 0) {
          sVar1 = *(short *)(unaff_A6 + 0x21);
          uVar3 = sVar1 == 0;
          *(short *)(unaff_A6 + 0x21) = sVar1 + -1;
          if ((bool)uVar3) {
            *unaff_A6 = candidate_397cc;
            uVar3 = 0;
          }
        }
      }
      else {
        *(undefined2 *)(unaff_A6 + 0x21) = *(undefined2 *)((int)unaff_A6 + 0x82);
        uVar3 = false;
        *(ushort *)((int)unaff_A6 + 0x72) = *(ushort *)((int)unaff_A6 + 0x72) & 0xfffd;
        candidate_39f0c();
        if ((bool)uVar3) {
          *unaff_A6 = candidate_39796;
          uVar3 = 0;
        }
      }
      candidate_39700();
      if ((bool)uVar3) {
        *unaff_A6 = candidate_3973a;
        uVar3 = 0;
      }
    }
  }
  else {
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *unaff_A6 = 0x397b0;
    uVar3 = false;
    candidate_39d74();
    if ((bool)uVar3) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
      *(undefined1 *)(unaff_A6 + 0x24) = 0;
      *unaff_A6 = 0x39682;
      uVar3 = 0;
    }
  }
  candidate_39b4a();
  candidate_4c24();
  if ((bool)uVar3) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    else {
      *unaff_A6 = candidate_39820;
    }
  }
  if (*(char *)((int)unaff_A6 + 0x93) != '\0') {
    *(char *)((int)unaff_A6 + 0x93) = *(char *)((int)unaff_A6 + 0x93) + -1;
  }
  return;
}
