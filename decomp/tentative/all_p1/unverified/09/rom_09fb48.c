/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09fb48.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9fb48(void)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  if (((*(byte *)(unaff_A6 + 0x24) & 0x80) == 0) ||
     ((*(short *)((int)unaff_A6 + 0x86) != 0 &&
      ((*(short *)((int)unaff_A6 + 0x86) < 1 ||
       (sVar1 = *(short *)((int)unaff_A6 + 0x86) + -1, *(short *)((int)unaff_A6 + 0x86) = sVar1,
       sVar1 != 0)))))) {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 1;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    if ((*(byte *)(unaff_A6 + 0x24) & 2) == 0) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *unaff_A6 = 0x9fba6;
    candidate_a0334();
    candidate_427a();
    if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
      *unaff_A6 = candidate_9fbc2;
    }
    if (((*(byte *)(unaff_A6 + 0x24) & 0x80) == 0) &&
       (*(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1,
       *(short *)(unaff_A6 + 0x20) < 1)) {
      *unaff_A6 = candidate_9fc9c;
    }
    uVar3 = false;
    candidate_a072a();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_9fade;
      uVar3 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = 0x9fca6;
      uVar3 = 0;
    }
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
    candidate_19dfc();
    uVar2 = 0x60000;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      uVar2 = 0xfffa0000;
    }
    *(undefined4 *)((int)unaff_A6 + 0x62) = uVar2;
    candidate_4256();
    *unaff_A6 = 0x9fce4;
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
      *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
      *unaff_A6 = 0x9fcfe;
    }
    uVar3 = 0;
    candidate_1456a();
    candidate_427a();
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9fd60;
  }
  return;
}
