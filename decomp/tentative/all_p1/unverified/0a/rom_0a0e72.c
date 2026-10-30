/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a0e72.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a0e72(void)

{
  uint uVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  *(undefined1 *)(unaff_A6 + 0x22) = 0x20;
  candidate_120f8();
  *(undefined1 *)(unaff_A6 + 0x18) = 4;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *unaff_A6 = 0xa0e92;
  if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
    candidate_1abc8();
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x18) = 2;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    *unaff_A6 = 0xa0eb4;
    if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
      candidate_a2312();
      candidate_a26b4();
      *unaff_A6 = 0xa0d5a;
      candidate_1abc8();
      uVar1 = unaff_A6[0xc];
      bVar3 = uVar1 < (uint)((int)unaff_A6[0xc] >> 3);
      unaff_A6[0xc] = uVar1 - ((int)unaff_A6[0xc] >> 3);
      candidate_a2560();
      if (!bVar3) {
        *unaff_A6 = candidate_a0d80;
      }
      candidate_a24c6();
      goto LAB_000a11fa;
    }
    candidate_1abc8();
  }
  candidate_1abc8();
  candidate_a244c();
LAB_000a11fa:
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
      uVar2 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar2;
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
