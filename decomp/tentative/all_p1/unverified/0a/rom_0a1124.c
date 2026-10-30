/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a1124.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a1124(void)

{
  undefined1 uVar2;
  undefined2 uVar1;
  undefined4 *unaff_A6;

  uVar2 = candidate_a22fc();
  *(undefined1 *)(unaff_A6 + 0x21) = uVar2;
  uVar2 = candidate_a22fc();
  *(undefined1 *)((int)unaff_A6 + 0x8b) = uVar2;
  uVar1 = candidate_a22fc();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar1;
  if ((*(byte *)(unaff_A6 + 0x21) & 1) == 0) {
    *(undefined1 *)(unaff_A6 + 0x18) = 0xc;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x18) = 0xd;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  }
  candidate_120f8();
  uVar1 = candidate_a22fc();
  *(undefined2 *)((int)unaff_A6 + 0x82) = uVar1;
  *unaff_A6 = 0xa1186;
  candidate_1abc8();
  *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  if ((*(short *)((int)unaff_A6 + 0x82) < 0) && (*(char *)((int)unaff_A6 + 0x61) != '\0')) {
    *unaff_A6 = candidate_a11c0;
    *(char *)(unaff_A6 + 0x21) = *(char *)(unaff_A6 + 0x21) + -1;
    if ('\0' < *(char *)(unaff_A6 + 0x21)) {
      *unaff_A6 = 0xa1148;
    }
  }
  candidate_1abc8();
  candidate_a241a();
  candidate_1ac38();
  if (*(short *)(unaff_A6 + 0x20) != 0) {
    *(short *)((int)unaff_A6 + 0x4e) =
         *(short *)((int)unaff_A6 + 0x4e) - *(short *)(unaff_A6 + 0x20);
    *(undefined2 *)(unaff_A6 + 0x20) = 0;
  }
  if (*(short *)((int)unaff_A6 + 0x4e) < 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
    *(undefined1 *)((int)unaff_A6 + 0x5e) = *(undefined1 *)(unaff_A6 + 0x24);
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a12c0;
  }
  *(char *)((int)unaff_A6 + 0x87) = *(char *)((int)unaff_A6 + 0x87) + '\x04';
  if (*(char *)(unaff_A6 + 0x23) != '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x17) = *(undefined1 *)(unaff_A6 + 0x23);
    *(undefined1 *)(unaff_A6 + 0x23) = 0;
    if (*(char *)((int)unaff_A6 + 0x8d) == '0') {
      *(undefined1 *)((int)unaff_A6 + 0x87) = 0xc2;
    }
  }
  if ('\0' < *(char *)((int)unaff_A6 + 0x17)) {
    *(char *)((int)unaff_A6 + 0x17) = *(char *)((int)unaff_A6 + 0x17) + -1;
  }
  if (*(char *)((int)unaff_A6 + 0x8d) != '\0') {
    *(char *)((int)unaff_A6 + 0x8d) = *(char *)((int)unaff_A6 + 0x8d) + -1;
  }
  if (*(short *)((int)unaff_A6 + 0x4e) <= *(short *)((int)unaff_A6 + 0x7e)) {
    if (*(char *)((int)unaff_A6 + 0x89) == '\0') {
      uVar1 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar1;
    }
    else {
      *(undefined2 *)((int)unaff_A6 + 0x7e) = 0xff00;
      candidate_a2926();
    }
    *(char *)((int)unaff_A6 + 0x89) = *(char *)((int)unaff_A6 + 0x89) + '\x01';
    candidate_a2312();
    candidate_a26b4();
  }
  *(undefined1 *)(unaff_A6 + 0x24) = 0;
  candidate_a2674();
  return;
}
