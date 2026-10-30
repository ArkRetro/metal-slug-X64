/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09fc9c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9fc9c(void)

{
  undefined4 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  candidate_19dfc();
  uVar1 = 0x60000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    uVar1 = 0xfffa0000;
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = uVar1;
  candidate_4256();
  *unaff_A6 = 0x9fce4;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    *unaff_A6 = 0x9fcfe;
  }
  uVar2 = 0;
  candidate_1456a();
  candidate_427a();
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_9fd60;
  }
  return;
}
