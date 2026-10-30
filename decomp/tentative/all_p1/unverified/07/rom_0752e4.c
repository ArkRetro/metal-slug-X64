/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0752e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_752e4(void)

{
  int iVar1;
  undefined4 *unaff_A6;

  candidate_9f88();
  candidate_9ea8();
  candidate_4520();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1110;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1110;
  iVar1 = (uint)*(ushort *)(unaff_A6[3] + 0x7e) * 0xc;
  unaff_A6[0x28] = *(undefined4 *)(iVar1 + 0xe3c34);
  *(undefined2 *)(unaff_A6 + 10) = *(undefined2 *)(iVar1 + 0xe3c38);
  *(undefined2 *)(unaff_A6 + 0xb) = *(undefined2 *)(iVar1 + 0xe3c3a);
  *(undefined2 *)((int)unaff_A6 + 0x76) = *(undefined2 *)(iVar1 + 0xe3c3c);
  if (*(char *)(iVar1 + 0xe3c3e) != '\0') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *(undefined1 *)((int)unaff_A6 + 0x12) = 0x80;
  *(undefined1 *)((int)unaff_A6 + 0x13) = 0x80;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined2 *)(unaff_A6 + 0x29) = 0;
  *(undefined1 *)((int)unaff_A6 + 0xa6) = 0;
  candidate_1ad52();
  candidate_120f8();
  candidate_40bc();
  candidate_1dcce();
  *unaff_A6 = candidate_75398;
  return;
}
