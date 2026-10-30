/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0671fc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x0006705a) */
/* WARNING: Removing unreachable block (ram,0x00067024) */
/* WARNING: Heritage AFTER dead removal. Example location: r0x0010c57d : 0x00067112 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void candidate_671fc(void)

{
  undefined4 uVar1;
  ushort uVar2;
  char cVar3;
  byte bVar4;
  undefined1 in_D0b;
  undefined1 in_D1b;
  int iVar5;
  int *in_A0;
  int *piVar6;
  int unaff_A5;
  ushort uStack_6;

  *(code **)(unaff_A5 + 0x458a) = candidate_3cf9a;
  *(undefined1 *)(unaff_A5 + 0x458e) = 0;
  uVar1 = *(undefined4 *)(unaff_A5 + 0x4586);
  _DAT_0010c582 = &piRam0010c572;
  _DAT_0010c586 = &piRam0010c572;
  uRam0010c579 = 0;
  uRam0010c57a = 0;
  uRam0010c581 = 0;
  uRam0010c57f = 3;
  uRam0010c580 = 0xff;
  bRam0010c57d = *(byte *)(in_A0 + 1);
  bRam0010c57e = *(byte *)((int)in_A0 + 5);
  cRam0010c578 = '\0';
  uStack_6 = 0;
  uRam0010c57b = in_D0b;
  uRam0010c57c = in_D1b;
  piRam0010c572 = in_A0;
  if (*in_A0 != 0) {
    uStack_6 = candidate_3d22a();
  }
  piVar6 = in_A0 + 4;
  iVar5 = *piVar6;
  while (iVar5 != -1) {
    if (((iVar5 != 0) && (iVar5 != -2)) && (uVar2 = candidate_3d22a(), uStack_6 < uVar2)) {
      uStack_6 = uVar2;
    }
    cRam0010c578 = cRam0010c578 + '\x01';
    piVar6 = piVar6 + 2;
    iVar5 = *piVar6;
  }
  if ((*(ushort *)((int)piRam0010c572 + 0xe) & 0xe) == 0) {
    cVar3 = '\x02';
  }
  else {
    cVar3 = '\0';
  }
  bVar4 = (char)uStack_6 + cVar3 + '\x02';
  if (bRam0010c57d < bVar4) {
    bRam0010c57d = bVar4;
  }
  if (bRam0010c57e < (byte)(cRam0010c578 + 2U)) {
    bRam0010c57e = cRam0010c578 + 2U;
  }
  uRam0010c576 = 0;
  uRam0010c58f = 1;
  _DAT_0010c586 = (int **)uVar1;
  return;
}
