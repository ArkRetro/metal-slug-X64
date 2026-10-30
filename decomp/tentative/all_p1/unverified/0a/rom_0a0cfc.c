/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a0cfc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a0cfc(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;

  sVar1 = 0;
  do {
    candidate_a2b08();
    sVar1 = sVar1 + 1;
  } while (sVar1 < 5);
  unaff_A6[0xd] = 0x10000;
  *unaff_A6 = 0xa0d1c;
  candidate_1abc8();
  candidate_1ac38();
  if (-0x29 < *(short *)(unaff_A6 + 0xb)) {
    unaff_A6[0xd] = 0;
    *unaff_A6 = candidate_a0d9c;
    *(undefined1 *)((int)unaff_A6 + 0x91) = 1;
    *(byte *)((int)unaff_A6 + 0x92) = *(byte *)((int)unaff_A6 + 0x92) & 0xfe;
  }
  candidate_a2680();
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
