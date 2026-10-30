/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02460a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2460a(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x8a) = *(undefined1 *)(unaff_A6[3] + 0x8a);
  if (*(char *)((int)unaff_A6 + 0x8a) == '\x02') {
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x200;
  }
  else {
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  }
  *unaff_A6 = 0x24644;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *(byte *)(unaff_A6[3] + 0x8d) = *(byte *)(unaff_A6[3] + 0x8d) & 0xfe;
    *unaff_A6 = 0x2427c;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  candidate_24cca();
  return;
}
