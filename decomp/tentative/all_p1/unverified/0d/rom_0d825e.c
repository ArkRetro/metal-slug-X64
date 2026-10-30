/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d825e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d825e(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) == 0) {
    *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
    uVar1 = false;
    if (0 < *(short *)((int)unaff_A6 + 0x76)) goto LAB_000d82c8;
    *(undefined2 *)((int)unaff_A6 + 0x76) = *(undefined2 *)(unaff_A6 + 0x1e);
    candidate_4256();
    *(char *)((int)unaff_A6 + 0x82) = *(char *)((int)unaff_A6 + 0x82) + -1;
    uVar1 = false;
    if ('\0' < *(char *)((int)unaff_A6 + 0x82)) goto LAB_000d82c8;
    *(undefined1 *)((int)unaff_A6 + 0x82) = *(undefined1 *)((int)unaff_A6 + 0x7f);
    *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + 1;
    uVar1 = *(ushort *)((int)unaff_A6 + 0x6e) < 10;
    if ((short)*(ushort *)((int)unaff_A6 + 0x6e) < 0xb) goto LAB_000d82c8;
    candidate_4256();
  }
  *unaff_A6 = 0xd81c4;
  uVar1 = false;
LAB_000d82c8:
  candidate_d9276();
  candidate_44f6();
  candidate_427a();
  if ((bool)uVar1) {
    candidate_d92ec();
    uVar1 = 0;
    if (*(char *)((int)unaff_A6 + 0x8a) == '\0') {
      *(undefined1 *)((int)unaff_A6 + 0x8a) = 0xff;
      uVar1 = 0;
      candidate_d922c();
    }
  }
  candidate_409a();
  if (!(bool)uVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
