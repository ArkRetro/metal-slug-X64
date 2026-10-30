/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x030e3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_30e3a(void)

{
  char cVar1;
  undefined2 uVar2;
  undefined4 uVar3;
  undefined4 *unaff_A6;

  uVar3 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x10) == 0) {
    if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x20) == 0) {
      if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x40) != 0) {
        uVar3 = 0x3087e;
      }
    }
    else {
      uVar3 = 0x3086f;
    }
  }
  else {
    candidate_120f8();
    uVar3 = 0x30867;
  }
  *(undefined4 *)((int)unaff_A6 + 0x7a) = uVar3;
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x5a;
  if (*(short *)(unaff_A6 + 0x20) == 0) {
    uVar2 = 4;
  }
  else {
    uVar2 = 0x15;
  }
  *(undefined2 *)(unaff_A6 + 10) = uVar2;
  *(undefined2 *)(unaff_A6 + 0xb) = 0x11;
  *unaff_A6 = 0x30ea8;
  if ((*(int *)((int)unaff_A6 + 0x7a) != 0) && ((_DAT_0010000e & 7) == 0)) {
    candidate_30162();
  }
  cVar1 = *(char *)(unaff_A6[3] + 0x61);
  *(char *)((int)unaff_A6 + 0x61) = cVar1;
  if (cVar1 != '\0') {
    *(undefined2 *)((int)unaff_A6 + 0x7e) = 1;
  }
  *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
  if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
    if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x10) != 0) {
      *(int *)((int)unaff_A6 + 0x72) = *(int *)((int)unaff_A6 + 0x72) + 0x100000;
      candidate_120f8();
      candidate_303de();
      *(undefined4 *)((int)unaff_A6 + 0x76) = 0x100000;
      candidate_57948();
    }
    if (*(int *)((int)unaff_A6 + 0x7a) != 0) {
      candidate_30162();
    }
    *unaff_A6 = candidate_30f76;
  }
  if ((*(byte *)((int)unaff_A6 + 0x8f) & 0x80) == 0) {
    *unaff_A6 = candidate_30f76;
  }
  *(char *)(unaff_A6[3] + 0x60) = *(char *)(unaff_A6[3] + 0x60) + '\x01';
  candidate_427a();
  return;
}
