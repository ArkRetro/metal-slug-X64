/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04e5be.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x0004e5be: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x0004e5c0) */
/* WARNING: Removing unreachable block (ram,0x0004e578) */
/* WARNING: Removing unreachable block (ram,0x0004e5c4) */
/* WARNING: Removing unreachable block (ram,0x0004e5ca) */
/* WARNING: Removing unreachable block (ram,0x0004e5ce) */

undefined8 candidate_4e5be(void)

{
  int iVar1;
  uint in_D0;
  uint in_D1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  ushort uStack_c;
  undefined2 uStack_a;
  undefined2 uStack_8;
  undefined2 uStack_6;

  uVar2 = (in_D1 & 0xffff) * (in_D0 & 0xffff);
  uVar4 = uVar2 >> 0x10;
  uStack_6 = (undefined2)uVar2;
  uVar3 = (in_D0 >> 0x10) * (in_D1 & 0xffff);
  uVar2 = uVar3 + uVar4;
  uStack_c = (ushort)CARRY4(uVar3,uVar4);
  uVar4 = (in_D0 & 0xffff) * (in_D1 >> 0x10);
  iVar1 = uVar4 + uVar2;
  uStack_a = (undefined2)((uint)iVar1 >> 0x10);
  uStack_8 = (undefined2)iVar1;
  if (CARRY4(uVar4,uVar2)) {
    uStack_c = uStack_c + 1;
  }
  return CONCAT44(CONCAT22(uStack_8,uStack_6),
                  CONCAT22(uStack_c,uStack_a) + (in_D1 >> 0x10) * (in_D0 >> 0x10));
}
