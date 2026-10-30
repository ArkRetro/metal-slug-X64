/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03973a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3973a(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_40bc();
  candidate_1b242();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  candidate_39778();
  *unaff_A6 = 0x3975e;
  uVar2 = 0;
  if (*(char *)(unaff_A6 + 0x18) == '\0') {
    candidate_39d74();
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x93) = 0xf;
    candidate_39f06();
    candidate_39eec();
    candidate_3971c();
    *unaff_A6 = 0x39694;
    uVar2 = 0;
    candidate_39d74();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
        *unaff_A6 = candidate_39796;
        uVar2 = 0;
      }
      candidate_39e9a();
      if ((bool)uVar2) {
        uVar2 = 0;
        if (*(short *)((int)unaff_A6 + 0x72) == 0) {
          sVar1 = *(short *)(unaff_A6 + 0x21);
          uVar2 = sVar1 == 0;
          *(short *)(unaff_A6 + 0x21) = sVar1 + -1;
          if ((bool)uVar2) {
            *unaff_A6 = candidate_397cc;
            uVar2 = 0;
          }
        }
      }
      else {
        *(undefined2 *)(unaff_A6 + 0x21) = *(undefined2 *)((int)unaff_A6 + 0x82);
        uVar2 = false;
        *(ushort *)((int)unaff_A6 + 0x72) = *(ushort *)((int)unaff_A6 + 0x72) & 0xfffd;
        candidate_39f0c();
        if ((bool)uVar2) {
          *unaff_A6 = candidate_39796;
          uVar2 = 0;
        }
      }
      candidate_39700();
      if ((bool)uVar2) {
        *unaff_A6 = candidate_3973a;
        uVar2 = 0;
      }
    }
  }
  candidate_39b4a();
  candidate_4c24();
  if ((bool)uVar2) {
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
